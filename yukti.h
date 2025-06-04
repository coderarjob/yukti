/*
 * Yukti Single Header Unittest and Mocking framework
 *
 * Author: Arjob Mukherjee (arjobmukherjee@gmail.com)
 * Version: 0.0.1-dev
 *
 * Mock/Fake function macros for Unit testing parts of a program. These macros make it easy to
 * create fake/mock functions, with support for return and byref parameters.
 *
 * ----------------------------------------------------------------------------
 * Examples:
 * ----------------------------------------------------------------------------
 *
 * C declaration              Mock declaration and definition
 * ------------------------   -----------------------------------------
 * void f ();                 YT_DECLARE_FUNC_VOID(f);
 *                            YT_DEFINE_FUNC_VOID(f);
 *
 * void f (int a);            YT_DECLARE_FUNC_VOID(f, int);
 *                            YT_DEFINE_FUNC_VOID(f, int);
 *
 * int f ();                  YT_DECLARE_FUNC(int, f);
 *                            YT_DEFINE_FUNC(int, f);
 *
 * int f (int a);             YT_DECLARE_FUNC(int, f, int);
 *                            YT_DEFINE_FUNC(int, f, int);
 */

#ifndef YUKTI_TEST_H
#define YUKTI_TEST_H

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

/*
 * =================================================================================
 * SECTION 0: SUPPORTING
 * =================================================================================
 * */

/* ----------------------------------------------------------------------------
 * Intrusive Linked List
 * ----------------------------------------------------------------------------*/
typedef struct ACL_ListNode {
    struct ACL_ListNode* next;
    struct ACL_ListNode* prev;
} ACL_ListNode;

static inline void acl_list_init (ACL_ListNode* node)
{
    node->next = node;
    node->prev = node;
}

static inline bool acl_list_is_empty (ACL_ListNode* head)
{
    return (head->next == head && head->prev == head);
}

static inline void acl_list_add_before (ACL_ListNode* node, ACL_ListNode* item)
{
    item->next       = node;
    item->prev       = node->prev;
    node->prev->next = item;
    node->prev       = item;
}

static inline void acl_list_add_after (ACL_ListNode* node, ACL_ListNode* item)
{
    item->prev       = node;
    item->next       = node->next;
    node->next->prev = item;
    node->next       = item;
}

static inline void acl_list_remove (ACL_ListNode* item)
{
    item->prev->next = item->next;
    item->next->prev = item->prev;
    item->next       = NULL;
    item->prev       = NULL;
}

#define ACL_LIST_ITEM(node, type, member) (type*)((uintptr_t)node - offsetof (type, member))

#define acl_list_for_each(head, node) \
    for ((node) = (head)->next; (node) != (head); (node) = (node)->next)

// ----------------------------------------------------------------------------
// Macros to find out the number of arguments passed to a variarg macros
// ----------------------------------------------------------------------------
#define YT_PRI_FIFTEENTH_ELEMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, \
                                 ...)                                                              \
    a15
#define YT_PRI_COUNT_ARGS(...) \
    YT_PRI_FIFTEENTH_ELEMENT (dummy, ##__VA_ARGS__, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// ----------------------------------------------------------------------------
// Color template for printing
// ----------------------------------------------------------------------------
#define YT_PRI_COL_RED   "\x1b[31m"
#define YT_PRI_COL_GREEN "\x1b[32m"
#define YT_PRI_COL_RESET "\x1b[0m"

#define YT_PRI_PASSED(t) printf ("\n  %sPass%s: %-20s", YT_PRI_COL_GREEN, YT_PRI_COL_RESET, #t)

#define YT_PRI_FAILED(t, fnt, ...)                                                     \
    do {                                                                               \
        printf ("\n  %s** FAIL ** %s: %-20s: ", YT_PRI_COL_RED, YT_PRI_COL_RESET, #t); \
        printf (fnt, ##__VA_ARGS__);                                                   \
    } while (0)

/*
 * =================================================================================
 * SECTION 1: MOCK FUNCTION CALL EXPECTATION MACROS
 * =================================================================================
 * */

#define YT_MUST_CALL_IN_ORDER(f, ...)                                                   \
    do {                                                                                \
        yt_pri_add_callrecord (&yt_pri_orderedExceptationListHead,                      \
                               YT_PRI_COUNT_ARGS (__VA_ARGS__) / 2, #f, ##__VA_ARGS__); \
    } while (0)

#define YT_MUST_CALL_ANY_ORDER(f, ...)                                                  \
    do {                                                                                \
        yt_pri_add_callrecord (&yt_pri_globalExceptationListHead,                       \
                               YT_PRI_COUNT_ARGS (__VA_ARGS__) / 2, #f, ##__VA_ARGS__); \
    } while (0)

#define YT_PRI_RECORD_CALL_X(n, ...)  YT_PRI_RECORD_CALL_##n (__VA_ARGS__)
#define YT_PRI_RECORD_CALL_10(t, ...) YT_V (t), YT_PRI_RECORD_CALL_9 (__VA_ARGS__)
#define YT_PRI_RECORD_CALL_9(t, ...)  YT_V (t), YT_PRI_RECORD_CALL_8 (__VA_ARGS__)
#define YT_PRI_RECORD_CALL_8(t, ...)  YT_V (t), YT_PRI_RECORD_CALL_7 (__VA_ARGS__)
#define YT_PRI_RECORD_CALL_7(t, ...)  YT_V (t), YT_PRI_RECORD_CALL_6 (__VA_ARGS__)
#define YT_PRI_RECORD_CALL_6(t, ...)  YT_V (t), YT_PRI_RECORD_CALL_5 (__VA_ARGS__)
#define YT_PRI_RECORD_CALL_5(t, ...)  YT_V (t), YT_PRI_RECORD_CALL_4 (__VA_ARGS__)
#define YT_PRI_RECORD_CALL_4(t, ...)  YT_V (t), YT_PRI_RECORD_CALL_3 (__VA_ARGS__)
#define YT_PRI_RECORD_CALL_3(t, ...)  YT_V (t), YT_PRI_RECORD_CALL_2 (__VA_ARGS__)
#define YT_PRI_RECORD_CALL_2(t, ...)  YT_V (t), YT_PRI_RECORD_CALL_1 (__VA_ARGS__)
#define YT_PRI_RECORD_CALL_1(t, ...)  YT_V (t)
#define YT_PRI_RECORD_CALL_0(...)

#define YT_PRI_RECORD_CALL(n, f, ...)                                                       \
    do {                                                                                    \
        yt_pri_add_callrecord (&yt_pri_actualCallListHead, YT_PRI_COUNT_ARGS (__VA_ARGS__), \
                               #f __VA_OPT__ (, ) YT_PRI_RECORD_CALL_X (n, __VA_ARGS__));   \
    } while (0)

#define YT_V(v)                    \
    (YT_PRI_Arg)                   \
    {                              \
        .isOpt = false, .val = (v) \
    }
#define _                       \
    (YT_PRI_Arg)                \
    {                           \
        .isOpt = true, .val = 0 \
    }

#define YT_PRI_MAX_STRING_SIZE    250
#define YT_PRI_ARG_OPTIONAL_CHAR  '!'
#define YT_PRI_ARG_SEPARATOR_CHAR ','

#define YT_PRI_PANIC(str)                                            \
    do {                                                             \
        printf ("YT_PRI_PANIC! %s.\n", (str) == NULL ? "" : str);    \
        printf ("   at %s:%d in %s.", __func__, __LINE__, __FILE__); \
        exit (1);                                                    \
    } while (0)

typedef struct YT_PRI_Arg {
    bool isOpt;
    int val;
} YT_PRI_Arg;

typedef struct YT_PRI_CallRecord {
    char callString[YT_PRI_MAX_STRING_SIZE];
    enum {
        YT_CALLRECORD_TYPE_ORDERED_EXPECTATION,
        YT_CALLRECORD_TYPE_GLOBAL_EXPECTATION,
        YT_CALLRECORD_TYPE_ACTUALCALL
    } type;
    ACL_ListNode listNode;
} YT_PRI_CallRecord;

static bool yt_pri_match_call_strings (const char* exp, const char* actual);
static void yt_pri_string_append (char* str, size_t size, const char* const fmt, ...);
static void yt_pri_call_record_free (YT_PRI_CallRecord* node);
#ifdef YUKTI_DEBUG
static void yt_pri_create_call_string (ACL_ListNode* head, char* buffer, size_t buffer_size, int n,
                                       const char* const fn, va_list l);
static void yt_pri_print_expectations (void);
#else
static void yt_pri_create_call_string (char* buffer, size_t buffer_size, int n,
                                       const char* const fn, va_list l);
#endif

static void yt_pri_add_callrecord (ACL_ListNode* head, int n, const char* const fn, ...);
static void yt_pri_print_unmet_expectations();
static void yt_pri_validate_expectations();
static void yt_pri_ec_init();

static ACL_ListNode yt_pri_orderedExceptationListHead;
static ACL_ListNode yt_pri_globalExceptationListHead;
static ACL_ListNode yt_pri_actualCallListHead;

#ifdef YUKTI_TEST_IMPLEMENTATION
static bool yt_pri_match_call_strings (const char* exp, const char* actual)
{
    // Expectation: Input pointers are not NULL. They are not user facing!
    assert (actual != NULL && exp != NULL);

    for (; *exp & *actual; exp++) {
        // Expectation: Its not possible to have an optional argument in actual calls
        assert (*actual != YT_PRI_ARG_OPTIONAL_CHAR);

        if (*exp == YT_PRI_ARG_OPTIONAL_CHAR) {
            for (; *actual && *actual != YT_PRI_ARG_SEPARATOR_CHAR && *actual != ')'; actual++)
                ;
            continue;
        } else if (*exp != *actual) {
            break;
        }
        actual++;
    }

    return *actual == *exp;
}

static void yt_pri_string_append (char* str, size_t size, const char* const fmt, ...)
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
        YT_PRI_PANIC (NULL);
    }

    if ((size_t)s >= size) {
        YT_PRI_PANIC ("Buffer size too small");
    }

    va_end (l);
}

static void yt_pri_call_record_free (YT_PRI_CallRecord* node)
{
    // Expectation: Input pointers are not NULL. They are not user facing!
    assert (node != NULL);

    acl_list_remove (&node->listNode);
    free (node);
}

    #ifdef YUKTI_DEBUG
static void yt_pri_create_call_string (ACL_ListNode* head, char* buffer, size_t buffer_size, int n,
                                       const char* const fn, va_list l)
    #else
static void yt_pri_create_call_string (char* buffer, size_t buffer_size, int n,
                                       const char* const fn, va_list l)
    #endif
{
    // Expectation: Input pointers are not NULL and Buffer size > 0. They are not user facing!
    assert (buffer != NULL && fn != NULL && buffer_size > 0);

    yt_pri_string_append (buffer, buffer_size, "%s(", fn);

    for (int i = 0; i < n; i++) {
        YT_PRI_Arg item = va_arg (l, YT_PRI_Arg);
        char separator  = (i == 0) ? ' ' : YT_PRI_ARG_SEPARATOR_CHAR;

        if (item.isOpt) {
    #ifdef YUKTI_DEBUG
            // Expectation: Actual call list must not have optional arguments
            assert (head != &yt_pri_actualCallListHead);
    #endif
            yt_pri_string_append (buffer, buffer_size, "%c%c", separator, YT_PRI_ARG_OPTIONAL_CHAR);
        } else {
            yt_pri_string_append (buffer, buffer_size, "%c%d", separator, item.val);
        }
    }
    yt_pri_string_append (buffer, buffer_size, ")");
    va_end (l);
}

void yt_pri_add_callrecord (ACL_ListNode* head, int n, const char* const fn, ...)
{
    YT_PRI_CallRecord* newrec = NULL;
    if (!(newrec = malloc (sizeof (YT_PRI_CallRecord)))) {
        YT_PRI_PANIC (NULL);
    }

    if (head == &yt_pri_orderedExceptationListHead) {
        newrec->type = YT_CALLRECORD_TYPE_ORDERED_EXPECTATION;
    } else if (head == &yt_pri_globalExceptationListHead) {
        newrec->type = YT_CALLRECORD_TYPE_GLOBAL_EXPECTATION;
    } else if (head == &yt_pri_actualCallListHead) {
        newrec->type = YT_CALLRECORD_TYPE_ACTUALCALL;
    } else {
        YT_PRI_PANIC ("Invalid list");
    }

    acl_list_init (&newrec->listNode);
    acl_list_add_before (head, &newrec->listNode);
    newrec->callString[0] = '\0';

    va_list l;
    va_start (l, fn);
    #ifdef YUKTI_DEBUG
    yt_pri_create_call_string (head, newrec->callString, sizeof (newrec->callString), n, fn, l);
    #else
    yt_pri_create_call_string (newrec->callString, sizeof (newrec->callString), n, fn, l);
    #endif
    va_end (l);
}

    #ifdef YUKTI_DEBUG
static void yt_pri_print_expectations (void)
{
    ACL_ListNode* node;
    printf ("\n-----------------\n");
    printf ("Global Expectation List (First to last)\n");
    acl_list_for_each (&yt_pri_globalExceptationListHead, node)
    {
        YT_PRI_CallRecord* item = ACL_LIST_ITEM (node, YT_PRI_CallRecord, listNode);
        printf ("    %s\n", item->callString);
    }
    printf ("-----------------\n");
    printf ("Ordered Expectation List (First to last)\n");
    acl_list_for_each (&yt_pri_orderedExceptationListHead, node)
    {
        YT_PRI_CallRecord* item = ACL_LIST_ITEM (node, YT_PRI_CallRecord, listNode);
        printf ("    %s\n", item->callString);
    }
    printf ("-----------------\n");
    printf ("Actual call List (First to last)\n");
    acl_list_for_each (&yt_pri_actualCallListHead, node)
    {
        YT_PRI_CallRecord* item = ACL_LIST_ITEM (node, YT_PRI_CallRecord, listNode);
        printf ("    %s\n", item->callString);
    }
    printf ("-----------------\n");
}
    #endif // YUKTI_DEBUG

void yt_pri_print_unmet_expectations()
{
    ACL_ListNode* node;
    // printf ("\n-----[Call expectations not met]-----\n");
    #ifdef YUKTI_DEBUG
    printf ("\nYUKTI_DEBUG: Actual order of functions calls:\n");
    acl_list_for_each (&yt_pri_actualCallListHead, node)
    {
        YT_PRI_CallRecord* item = ACL_LIST_ITEM (node, YT_PRI_CallRecord, listNode);

        // Actual List must contain only call records of actual calls
        assert (item->type == YT_CALLRECORD_TYPE_ACTUALCALL);

        printf ("* %s\n", item->callString);
    }
    #endif
    if (!acl_list_is_empty (&yt_pri_globalExceptationListHead)) {
        acl_list_for_each (&yt_pri_globalExceptationListHead, node)
        {
            YT_PRI_CallRecord* item = ACL_LIST_ITEM (node, YT_PRI_CallRecord, listNode);

            // Global List must contain only call records of global/unordered call expectations
            assert (item->type == YT_CALLRECORD_TYPE_GLOBAL_EXPECTATION);

            YT_PRI_FAILED (Expectation not met, "Was never called: %s", item->callString);
        }
    }
    if (!acl_list_is_empty (&yt_pri_orderedExceptationListHead)) {
        acl_list_for_each (&yt_pri_orderedExceptationListHead, node)
        {
            YT_PRI_CallRecord* item = ACL_LIST_ITEM (node, YT_PRI_CallRecord, listNode);

            // Ordered List must contain only call records of Ordered call expectations
            assert (item->type == YT_CALLRECORD_TYPE_ORDERED_EXPECTATION);

            YT_PRI_FAILED (Expectation not met, "Was never called/called out of order: %s",
                           item->callString);
        }
    }
    // printf ("\n-----------------");
}

void yt_pri_validate_expectations()
{
    ACL_ListNode* actCallNode;
    acl_list_for_each (&yt_pri_actualCallListHead, actCallNode)
    {
        YT_PRI_CallRecord* item = ACL_LIST_ITEM (actCallNode, YT_PRI_CallRecord, listNode);

        // Actual List must contain only call records of actual calls
        assert (item->type == YT_CALLRECORD_TYPE_ACTUALCALL);

        if (!acl_list_is_empty (&yt_pri_orderedExceptationListHead)) {
            YT_PRI_CallRecord* ordExp = ACL_LIST_ITEM (yt_pri_orderedExceptationListHead.next,
                                                       YT_PRI_CallRecord, listNode);

            // Ordered List must contain only call records of Ordered call expectations
            assert (ordExp->type == YT_CALLRECORD_TYPE_ORDERED_EXPECTATION);

            if (yt_pri_match_call_strings (ordExp->callString, item->callString)) {
                yt_pri_call_record_free (ordExp);
            }
        }

        ACL_ListNode* globalCallNode;
        acl_list_for_each (&yt_pri_globalExceptationListHead, globalCallNode)
        {
            YT_PRI_CallRecord* gloExp = ACL_LIST_ITEM (globalCallNode, YT_PRI_CallRecord, listNode);

            // Global List must contain only call records of global/unordered call expectations
            assert (gloExp->type == YT_CALLRECORD_TYPE_GLOBAL_EXPECTATION);

            if (yt_pri_match_call_strings (gloExp->callString, item->callString)) {
                yt_pri_call_record_free (gloExp);
                break;
            }
        }
    }

    bool success = acl_list_is_empty (&yt_pri_orderedExceptationListHead) &&
                   acl_list_is_empty (&yt_pri_globalExceptationListHead);

    if (!success) {
        yt_pri_print_unmet_expectations();
    }
}

void yt_pri_ec_init()
{
    acl_list_init (&yt_pri_globalExceptationListHead);
    acl_list_init (&yt_pri_orderedExceptationListHead);
    acl_list_init (&yt_pri_actualCallListHead);
}
#endif // YUKTI_TEST_IMPLEMENTATION

/*
 * =================================================================================
 * SECTION 2: MOCK FUNCTION DECLARATION AND DEFINITION MACROS
 * =================================================================================
 * */

void reset(); // MUST BE DEFINED BY THE USER OF fake.h

// ----------------------------------------------------------------------------
// Common Helper macros used by both declaration and defination macros.
// ----------------------------------------------------------------------------
#define YT_PRI_STRUCT_TAG(f)     f##_fake_tag
#define YT_PRI_STRUCT_VAR(f)     f##_fake
#define YT_PRI_STRUCT_HANDLER(f) f##_fake_handler

// ---------------------FAKE FUNCTION DEFINITION ]-----------------------------
#define YT_DEFINE_FUNC_VOID(f, ...) \
    YT_PRI_DEFINE_FUNC_STRUCT (f);  \
    YT_PRI_DEFINE_FUNC_BODY_VOID (YT_PRI_COUNT_ARGS (__VA_ARGS__), f, __VA_ARGS__)

// ----
#define YT_DEFINE_FUNC(rt, f, ...) \
    YT_PRI_DEFINE_FUNC_STRUCT (f); \
    YT_PRI_DEFINE_FUNC_BODY (YT_PRI_COUNT_ARGS (__VA_ARGS__), rt, f, __VA_ARGS__)

// ----
#define YT_PRI_DEFINE_FUNC_STRUCT(f) YT_PRI_STRUCT_TAG (f) YT_PRI_STRUCT_VAR (f) = {}

#define YT_PRI_DEFINE_FUNC_BODY_VOID(n, f, ...)    \
    void f (YT_PRI_FUNC_PARAMS_X (n, __VA_ARGS__)) \
    {                                              \
        YT_PRI_RECORD_CALL (n, f, __VA_ARGS__);    \
        YT_PRI_STRUCT_VAR (f).invokeCount++;       \
        YT_PRI_RETURN_VOID (n, f, __VA_ARGS__);    \
    }

#define YT_PRI_DEFINE_FUNC_BODY(n, rt, f, ...)   \
    rt f (YT_PRI_FUNC_PARAMS_X (n, __VA_ARGS__)) \
    {                                            \
        YT_PRI_RECORD_CALL (n, f, __VA_ARGS__);  \
        YT_PRI_STRUCT_VAR (f).invokeCount++;     \
        YT_PRI_RETURN (n, f, __VA_ARGS__);       \
    }

#define YT_PRI_RETURN_VOID(n, f, ...)  \
    if (YT_PRI_STRUCT_VAR (f).handler) \
    YT_PRI_STRUCT_VAR (f).handler (YT_PRI_FUNC_ARG_##n())

#define YT_PRI_RETURN(n, f, ...)                                                                   \
    return (YT_PRI_STRUCT_VAR (f).handler) ? YT_PRI_STRUCT_VAR (f).handler (YT_PRI_FUNC_ARG_##n()) \
                                           : YT_PRI_STRUCT_VAR (f).ret

#define YT_PRI_FUNC_PARAMS_X(n, ...) YT_PRI_FUNC_PARAMS_##n (__VA_ARGS__)
#define YT_PRI_FUNC_PARAMS_4(t, ...) t d, YT_PRI_FUNC_PARAMS_3 (__VA_ARGS__)
#define YT_PRI_FUNC_PARAMS_3(t, ...) t c, YT_PRI_FUNC_PARAMS_2 (__VA_ARGS__)
#define YT_PRI_FUNC_PARAMS_2(t, ...) t b, YT_PRI_FUNC_PARAMS_1 (__VA_ARGS__)
#define YT_PRI_FUNC_PARAMS_1(t, ...) t a
#define YT_PRI_FUNC_PARAMS_0(...)

#define YT_PRI_FUNC_ARG_4() d, YT_PRI_FUNC_PARAMS_3()
#define YT_PRI_FUNC_ARG_3() c, YT_PRI_FUNC_PARAMS_2()
#define YT_PRI_FUNC_ARG_2() b, YT_PRI_FUNC_PARAMS_1()
#define YT_PRI_FUNC_ARG_1() a
#define YT_PRI_FUNC_ARG_0()

// -----------------------[ FAKE FUNCTION DECLARATION ]------------------------
#define YT_DECLARE_FUNC_VOID(f, ...)                \
    typedef void (*f##_fake_handler) (__VA_ARGS__); \
    YT_PRI_DECLARE_STRUCT_VOID (f)

// ----
#define YT_DECLARE_FUNC(rt, f, ...)               \
    typedef rt (*f##_fake_handler) (__VA_ARGS__); \
    YT_PRI_DECLARE_STRUCT (rt, f)

// ----
#define YT_PRI_DECLARE_STRUCT_VOID(f)                         \
    YT_PRI_DECLARE_STRUCT_START (f)                           \
    YT_PRI_STRUCT_FIELD (void*, resources);                   \
    YT_PRI_STRUCT_FIELD (unsigned int, invokeCount);          \
    YT_PRI_STRUCT_FIELD (YT_PRI_STRUCT_HANDLER (f), handler); \
    YT_PRI_DECLARE_STRUCT_END (f)

#define YT_PRI_DECLARE_STRUCT(rt, f)                          \
    YT_PRI_DECLARE_STRUCT_START (f)                           \
    YT_PRI_STRUCT_FIELD (void*, resources);                   \
    YT_PRI_STRUCT_FIELD (unsigned int, invokeCount);          \
    YT_PRI_STRUCT_FIELD (rt, ret);                            \
    YT_PRI_STRUCT_FIELD (YT_PRI_STRUCT_HANDLER (f), handler); \
    YT_PRI_DECLARE_STRUCT_END (f)

#define YT_PRI_DECLARE_STRUCT_START(f) typedef struct YT_PRI_STRUCT_TAG (f) {
#define YT_PRI_STRUCT_FIELD(pt, p)     pt p;
#define YT_PRI_DECLARE_STRUCT_END(f) \
    }                                \
    YT_PRI_STRUCT_TAG (f);           \
    extern YT_PRI_STRUCT_TAG (f) YT_PRI_STRUCT_VAR (f)

// -----------------------[ RESET FAKE ]------------------------
#define YT_RESET_FAKE(f) memset (&YT_PRI_STRUCT_VAR (f), 0, sizeof (YT_PRI_STRUCT_VAR (f)))

/*
 * =================================================================================
 * SECTION 3: UNITTEST FUNCTIONS & MACROS
 * =================================================================================
 * */

static int yt_pri_equal_mem (const void* a, const void* b, unsigned long size, int* i);
static int yt_pri_equal_string (const char* a, const char* b, int* i);

#define YT_PRI_TEST_SCALAR(a, o, b)                               \
    do {                                                          \
        __auto_type ut_a = (a);                                   \
        __auto_type ut_b = (b);                                   \
        if (ut_a o ut_b)                                          \
            YT_PRI_PASSED (a o b);                                \
        else                                                      \
            YT_PRI_FAILED (a o b, "[%d !" #o " %d]", ut_a, ut_b); \
    } while (0)

#define YT_PRI_TEST_MEM(a, o, b, sz)                                                    \
    do {                                                                                \
        __auto_type ut_a = (a);                                                         \
        __auto_type ut_b = (b);                                                         \
        int i;                                                                          \
        if (ut_equal_mem (ut_a, ut_b, sz, &i) o 1)                                      \
            YT_PRI_PASSED (a o b);                                                      \
        else                                                                            \
            YT_PRI_FAILED (a o b, "[Idx: %d, 0x%X !" #o " 0x%X]", i, ut_a[i], ut_b[i]); \
    } while (0)

#define YT_PRI_TEST_STRING(a, o, b)                                                     \
    do {                                                                                \
        __auto_type ut_a = (a);                                                         \
        __auto_type ut_b = (b);                                                         \
        int i;                                                                          \
        if (ut_equal_string (ut_a, ut_b, &i) o 1)                                       \
            YT_PRI_PASSED (a o b);                                                      \
        else                                                                            \
            YT_PRI_FAILED (a o b, "[Idx: %d, '%c' !" #o " '%c']", i, ut_a[i], ut_b[i]); \
    } while (0)

#define YT_EQ_SCALAR(a, b)  YT_PRI_TEST_SCALAR (a, ==, b)
#define YT_NEQ_SCALAR(a, b) YT_PRI_TEST_SCALAR (a, !=, b)
#define YT_GEQ_SCALAR(a, b) YT_PRI_TEST_SCALAR (a, >=, b)
#define YT_LEQ_SCALAR(a, b) YT_PRI_TEST_SCALAR (a, <=, b)
#define YT_LES_SCALAR(a, b) YT_PRI_TEST_SCALAR (a, <, b)
#define YT_GRT_SCALAR(a, b) YT_PRI_TEST_SCALAR (a, >, b)

#define YT_EQ_MEM(a, b, sz)  YT_PRI_TEST_MEM (a, ==, b, sz)
#define YT_NEQ_MEM(a, b, sz) YT_PRI_TEST_MEM (a, !=, b, sz)

#define YT_EQ_STRING(a, b)  YT_PRI_TEST_STRING (a, ==, b)
#define YT_NEQ_STRING(a, b) YT_PRI_TEST_STRING (a, !=, b)

#define YT_TEST(tf, fn)                    \
    static void fn()                       \
    {                                      \
        reset();                           \
        yt_pri_ec_init();                  \
        printf ("TEST (%s) %s", #tf, #fn); \
        do

#define YT_END()                    \
    yt_pri_validate_expectations(); \
    }                               \
    while (0)                       \
        ;                           \
    printf ("\n")

#ifdef YUKTI_TEST_IMPLEMENTATION
static int yt_pri_equal_string (const char* a, const char* b, int* i)
{
    *i = 0;
    while (*a && *b && *a == *b) {
        a++;
        b++;
        (*i)++;
    }

    return *a == *b;
}

static int yt_pri_equal_mem (const void* a, const void* b, unsigned long size, int* i)
{
    *i = 0;
    while (size-- && *(uint8_t*)a++ == *(uint8_t*)b++)
        (*i)++;

    return *(uint8_t*)--a == *(uint8_t*)--b;
}
#endif // YUKTI_TEST_IMPLEMENTATION

#endif // YUKTI_TEST_H
