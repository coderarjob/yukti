#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <list.h>

#define FIFTEENTH_ITEM(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, ...) a15
#define COUNT_ARGS(...) \
    FIFTEENTH_ITEM (dummy, ##__VA_ARGS__, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define MUST_CALL_ORDERED(f, ...)                                                               \
    do {                                                                                        \
        add_call_record_to_list (&orderedExceptationListHead, COUNT_ARGS (__VA_ARGS__) / 2, #f, \
                                 ##__VA_ARGS__);                                                \
    } while (0)

#define MUST_CALL(f, ...)                                                                      \
    do {                                                                                       \
        add_call_record_to_list (&globalExceptationListHead, COUNT_ARGS (__VA_ARGS__) / 2, #f, \
                                 ##__VA_ARGS__);                                               \
    } while (0)

#define RECORD_CALL(f, ...)                                                             \
    do {                                                                                \
        add_call_record_to_list (&actualCallListHead, COUNT_ARGS (__VA_ARGS__) / 2, #f, \
                                 ##__VA_ARGS__);                                        \
    } while (0)

#define V(v)                       \
    (Arg)                          \
    {                              \
        .isOpt = false, .val = (v) \
    }
#define _                       \
    (Arg)                       \
    {                           \
        .isOpt = true, .val = 0 \
    }

#define MAX_STRING_SIZE    250
#define ARG_OPTIONAL_CHAR  '!'
#define ARG_SEPARATOR_CHAR ','

#define PANIC(str)                                                   \
    do {                                                             \
        printf ("Panic! %s.\n", (str) == NULL ? "" : str);           \
        printf ("   at %s:%d in %s.", __func__, __LINE__, __FILE__); \
        exit (1);                                                    \
    } while (0)

typedef struct Arg {
    bool isOpt;
    int val;
} Arg;

typedef struct CallRecord {
    char callString[MAX_STRING_SIZE];
    enum {
        CALL_RECORD_TYPE_ORDERED_EXCEPTION,
        CALL_RECORD_TYPE_GLOBAL_EXCEPTION,
        CALL_RECORD_TYPE_ACTUAL_CALL
    } type;
    union {
        ListNode orderedExceptationListNode;
        ListNode globalExceptationListNode;
        ListNode actualCallListNode;
    } listnodes;
} CallRecord;

static bool match_call_strings (const char* exp, const char* actual);
static void string_append (char* str, size_t size, const char* const fmt, ...);
static void call_record_free (CallRecord* node);
#ifdef DEBUG
static void create_call_string (ListNode* head, char* buffer, size_t buffer_size, int n,
                                const char* const fn, va_list l);
static void print_expectations (void);
#else
static void create_call_string (char* buffer, size_t buffer_size, int n, const char* const fn,
                                va_list l);
    #define print_expectations(...) (void)0
#endif

void add_call_record_to_list (ListNode* head, int n, const char* const fn, ...);
void print_unmet_expectations();
bool validate_expectations();
void init();

static ListNode orderedExceptationListHead;
static ListNode globalExceptationListHead;
static ListNode actualCallListHead;

static bool match_call_strings (const char* exp, const char* actual)
{
    // Expectation: Input pointers are not NULL. They are not user facing!
    assert (actual != NULL && exp != NULL);

    for (; *exp & *actual; exp++) {
        // Expectation: Its not possible to have an optional argument in actual calls
        assert (*actual != ARG_OPTIONAL_CHAR);

        if (*exp == ARG_OPTIONAL_CHAR) {
            for (; *actual && *actual != ARG_SEPARATOR_CHAR && *actual != ')'; actual++)
                ;
            continue;
        } else if (*exp != *actual) {
            break;
        }
        actual++;
    }

    return *actual == *exp;
}

static void string_append (char* str, size_t size, const char* const fmt, ...)
{
    // Expectation: Input pointers are not NULL and Buffer size > 0. They are not user facing!
    assert (str != NULL && fmt != NULL && size > 0);

    size_t len = strnlen (str, size);

    // Expectation: Not possible for string in 'str' to be > size. Because of below check!
    assert (len <= size);

    size -= len;
    str += len;

    va_list l;
    va_start (l, fmt);

    int s = 0;
    if ((s = vsnprintf (str, size, fmt, l)) < 0) {
        perror ("snprintf");
        PANIC (NULL);
    }

    if ((size_t)s >= size) {
        PANIC ("Buffer size too small");
    }

    va_end (l);
}

static void call_record_free (CallRecord* node)
{
    // Expectation: Input pointers are not NULL. They are not user facing!
    assert (node != NULL);

    if (node->type == CALL_RECORD_TYPE_ORDERED_EXCEPTION) {
        list_remove (&node->listnodes.orderedExceptationListNode);
    } else if (node->type == CALL_RECORD_TYPE_GLOBAL_EXCEPTION) {
        list_remove (&node->listnodes.globalExceptationListNode);
    } else if (node->type == CALL_RECORD_TYPE_ACTUAL_CALL) {
        list_remove (&node->listnodes.actualCallListNode);
    } else {
        PANIC ("Invalid list");
    }
    free (node);
}

#ifdef DEBUG
static void create_call_string (ListNode* head, char* buffer, size_t buffer_size, int n,
                                const char* const fn, va_list l)
#else
static void create_call_string (char* buffer, size_t buffer_size, int n, const char* const fn,
                                va_list l)
#endif
{
    // Expectation: Input pointers are not NULL and Buffer size > 0. They are not user facing!
    assert (buffer != NULL && fn != NULL && buffer_size > 0);

    string_append (buffer, buffer_size, "%s", fn);

    for (int i = 0; i < n; i++) {
        Arg item       = va_arg (l, Arg);
        char separator = (i == 0) ? '(' : ARG_SEPARATOR_CHAR;

        if (item.isOpt) {
#ifdef DEBUG
            // Expectation: Actual call list must not have optional arguments
            assert (head != &actualCallListHead);
#endif
            string_append (buffer, buffer_size, "%c%c", separator, ARG_OPTIONAL_CHAR);
        } else {
            string_append (buffer, buffer_size, "%c%d", separator, item.val);
        }
    }
    string_append (buffer, buffer_size, ")");
    va_end (l);
}

void add_call_record_to_list (ListNode* head, int n, const char* const fn, ...)
{
    CallRecord* newrec = NULL;
    if (!(newrec = malloc (sizeof (CallRecord)))) {
        PANIC (NULL);
    }

    if (head == &orderedExceptationListHead) {
        newrec->type = CALL_RECORD_TYPE_ORDERED_EXCEPTION;
        list_init (&newrec->listnodes.orderedExceptationListNode);
        list_add_before (head, &newrec->listnodes.orderedExceptationListNode);
    } else if (head == &globalExceptationListHead) {
        newrec->type = CALL_RECORD_TYPE_GLOBAL_EXCEPTION;
        list_init (&newrec->listnodes.globalExceptationListNode);
        list_add_before (head, &newrec->listnodes.globalExceptationListNode);
    } else if (head == &actualCallListHead) {
        newrec->type = CALL_RECORD_TYPE_ACTUAL_CALL;
        list_init (&newrec->listnodes.actualCallListNode);
        list_add_before (head, &newrec->listnodes.actualCallListNode);
    } else {
        PANIC ("Invalid list");
    }

    newrec->callString[0] = '\0';

    va_list l;
    va_start (l, fn);
#ifdef DEBUG
    create_call_string (head, newrec->callString, sizeof (newrec->callString), n, fn, l);
#else
    create_call_string (newrec->callString, sizeof (newrec->callString), n, fn, l);
#endif
    va_end (l);
}

#ifdef DEBUG
static void print_expectations (void)
{
    ListNode* node;
    printf ("-----------------\n");
    printf ("Global Expectation List (First to last)\n");
    list_for_each (&globalExceptationListHead, node)
    {
        CallRecord* item = LIST_ITEM (node, CallRecord, listnodes.globalExceptationListNode);
        printf ("    %s\n", item->callString);
    }
    printf ("-----------------\n");
    printf ("Ordered Expectation List (First to last)\n");
    list_for_each (&orderedExceptationListHead, node)
    {
        CallRecord* item = LIST_ITEM (node, CallRecord, listnodes.orderedExceptationListNode);
        printf ("    %s\n", item->callString);
    }
    printf ("-----------------\n");
    printf ("Actual call List (First to last)\n");
    list_for_each (&actualCallListHead, node)
    {
        CallRecord* item = LIST_ITEM (node, CallRecord, listnodes.actualCallListNode);
        printf ("    %s\n", item->callString);
    }
    printf ("-----------------\n");
}
#endif // DEBUG

void print_unmet_expectations()
{
    ListNode* node;
    printf ("-----------------\n");
#ifdef DEBUG
    printf ("DEBUG: Functions calls in the order they happened:\n");
    list_for_each (&actualCallListHead, node)
    {
        CallRecord* item = LIST_ITEM (node, CallRecord, listnodes.actualCallListNode);
        printf ("* %s\n", item->callString);
    }
#endif
    if (!list_is_empty (&globalExceptationListHead)) {
        printf ("-----------------\n");
        printf ("To be called once anytime in any order (was not called):\n");
        list_for_each (&globalExceptationListHead, node)
        {
            CallRecord* item = LIST_ITEM (node, CallRecord, listnodes.globalExceptationListNode);
            printf ("* %s\n", item->callString);
        }
    }
    if (!list_is_empty (&orderedExceptationListHead)) {
        printf ("-----------------\n");
        printf ("To be called once in an order (was not called):\n");
        list_for_each (&orderedExceptationListHead, node)
        {
            CallRecord* item = LIST_ITEM (node, CallRecord, listnodes.orderedExceptationListNode);
            printf ("* %s\n", item->callString);
        }
    }
    printf ("-----------------\n");
}

bool validate_expectations()
{
    ListNode* actCallNode;
    list_for_each (&actualCallListHead, actCallNode)
    {
        CallRecord* item = LIST_ITEM (actCallNode, CallRecord, listnodes.actualCallListNode);

        if (!list_is_empty (&orderedExceptationListHead)) {
            CallRecord* ordExp = LIST_ITEM (orderedExceptationListHead.next, CallRecord,
                                            listnodes.orderedExceptationListNode);
            if (match_call_strings (ordExp->callString, item->callString)) {
                call_record_free (ordExp);
            }
        }

        ListNode* globalCallNode;
        list_for_each (&globalExceptationListHead, globalCallNode)
        {
            CallRecord* gloExp = LIST_ITEM (globalCallNode, CallRecord,
                                            listnodes.globalExceptationListNode);
            if (match_call_strings (gloExp->callString, item->callString)) {
                call_record_free (gloExp);
                break;
            }
        }
    }

    return list_is_empty (&orderedExceptationListHead) &&
           list_is_empty (&globalExceptationListHead);
}

void init()
{
    list_init (&globalExceptationListHead);
    list_init (&orderedExceptationListHead);
    list_init (&actualCallListHead);
}
