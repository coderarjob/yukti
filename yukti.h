/*
 * Yukti Single Header Unittest and Mocking framework
 *
 * Author: Arjob Mukherjee (arjobmukherjee@gmail.com)
 * Version: 0.1.0
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
 * ========================================================================================
 * SECTION 0: SUPPORTING
 * ========================================================================================
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
#define YT_PRI_COUNT_ARGS(...)                                                                     \
    YT_PRI_FIFTEENTH_ELEMENT (dummy __VA_OPT__ (, ) __VA_ARGS__, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, \
                              3, 2, 1, 0)

// ----------------------------------------------------------------------------
// Color template for printing
// ----------------------------------------------------------------------------
#define YT_PRI_COL_GRAY      "\x1b[90m"
#define YT_PRI_COL_RED       "\x1b[31m"
#define YT_PRI_COL_GREEN     "\x1b[32m"
#define YT_PRI_COL_HIGHLIGHT "\x1b[97;100m"
#define YT_PRI_COL_RESET     "\x1b[0m"

#define YT_PRI_PASSED(t) printf ("\n  %sPass%s: %-20s", YT_PRI_COL_GREEN, YT_PRI_COL_RESET, #t)

#define YT_PRI_FAILED(t, fnt, ...)                                                     \
    do {                                                                               \
        yt_pri_current_testrecord->failed_exp_count++;                                 \
        printf ("\n  %s** FAIL ** %s: %-20s: ", YT_PRI_COL_RED, YT_PRI_COL_RESET, #t); \
        printf (fnt, ##__VA_ARGS__);                                                   \
    } while (0)

#define YT_PRI_PANIC(str)                                            \
    do {                                                             \
        printf ("YT_PRI_PANIC! %s.\n", (str) == NULL ? "" : str);    \
        printf ("   at %s:%d in %s.", __func__, __LINE__, __FILE__); \
        exit (1);                                                    \
    } while (0)

// ----------------------------------------------------------------------------
// Declaring, defining and calling functions
// ----------------------------------------------------------------------------
#define YT_PRI_FCALL_WRAP_ARGS_X(n, w, ...)  YT_PRI_FCALL_WRAP_ARGS_##n (w, __VA_ARGS__)
#define YT_PRI_FCALL_WRAP_ARGS_10(w, t, ...) YT_PRI_FCALL_WRAP_ARGS_9 (w, __VA_ARGS__), w (_j)
#define YT_PRI_FCALL_WRAP_ARGS_9(w, t, ...)  YT_PRI_FCALL_WRAP_ARGS_8 (w, __VA_ARGS__), w (_i)
#define YT_PRI_FCALL_WRAP_ARGS_8(w, t, ...)  YT_PRI_FCALL_WRAP_ARGS_7 (w, __VA_ARGS__), w (_h)
#define YT_PRI_FCALL_WRAP_ARGS_7(w, t, ...)  YT_PRI_FCALL_WRAP_ARGS_6 (w, __VA_ARGS__), w (_g)
#define YT_PRI_FCALL_WRAP_ARGS_6(w, t, ...)  YT_PRI_FCALL_WRAP_ARGS_5 (w, __VA_ARGS__), w (_f)
#define YT_PRI_FCALL_WRAP_ARGS_5(w, t, ...)  YT_PRI_FCALL_WRAP_ARGS_4 (w, __VA_ARGS__), w (_e)
#define YT_PRI_FCALL_WRAP_ARGS_4(w, t, ...)  YT_PRI_FCALL_WRAP_ARGS_3 (w, __VA_ARGS__), w (_d)
#define YT_PRI_FCALL_WRAP_ARGS_3(w, t, ...)  YT_PRI_FCALL_WRAP_ARGS_2 (w, __VA_ARGS__), w (_c)
#define YT_PRI_FCALL_WRAP_ARGS_2(w, t, ...)  YT_PRI_FCALL_WRAP_ARGS_1 (w, __VA_ARGS__), w (_b)
#define YT_PRI_FCALL_WRAP_ARGS_1(w, t, ...)  w (_a)
#define YT_PRI_FCALL_WRAP_ARGS_0(...)

#define YT_PRI_FCALL_ARGS_X(n, ...)  YT_PRI_FCALL_ARGS_##n (i, __VA_ARGS__)
#define YT_PRI_FCALL_ARGS_10(t, ...) YT_PRI_FCALL_ARGS_9 (i, __VA_ARGS__), _j
#define YT_PRI_FCALL_ARGS_9(t, ...)  YT_PRI_FCALL_ARGS_8 (i, __VA_ARGS__), _i
#define YT_PRI_FCALL_ARGS_8(t, ...)  YT_PRI_FCALL_ARGS_7 (i, __VA_ARGS__), _h
#define YT_PRI_FCALL_ARGS_7(t, ...)  YT_PRI_FCALL_ARGS_6 (i, __VA_ARGS__), _g
#define YT_PRI_FCALL_ARGS_6(t, ...)  YT_PRI_FCALL_ARGS_5 (i, __VA_ARGS__), _f
#define YT_PRI_FCALL_ARGS_5(t, ...)  YT_PRI_FCALL_ARGS_4 (i, __VA_ARGS__), _e
#define YT_PRI_FCALL_ARGS_4(t, ...)  YT_PRI_FCALL_ARGS_3 (i, __VA_ARGS__), _d
#define YT_PRI_FCALL_ARGS_3(t, ...)  YT_PRI_FCALL_ARGS_2 (i, __VA_ARGS__), _c
#define YT_PRI_FCALL_ARGS_2(t, ...)  YT_PRI_FCALL_ARGS_1 (i, __VA_ARGS__), _b
#define YT_PRI_FCALL_ARGS_1(t, ...)  _a
#define YT_PRI_FCALL_ARGS_0(...)

#define YT_PRI_FCALL_ARGS_ARRAY_X(n, i, ...)  YT_PRI_FCALL_ARGS_ARRAY_##n (i, __VA_ARGS__)
#define YT_PRI_FCALL_ARGS_ARRAY_10(i, t, ...) YT_PRI_FCALL_ARGS_ARRAY_9 (i, __VA_ARGS__), _j[i]
#define YT_PRI_FCALL_ARGS_ARRAY_9(i, t, ...)  YT_PRI_FCALL_ARGS_ARRAY_8 (i, __VA_ARGS__), _i[i]
#define YT_PRI_FCALL_ARGS_ARRAY_8(i, t, ...)  YT_PRI_FCALL_ARGS_ARRAY_7 (i, __VA_ARGS__), _h[i]
#define YT_PRI_FCALL_ARGS_ARRAY_7(i, t, ...)  YT_PRI_FCALL_ARGS_ARRAY_6 (i, __VA_ARGS__), _g[i]
#define YT_PRI_FCALL_ARGS_ARRAY_6(i, t, ...)  YT_PRI_FCALL_ARGS_ARRAY_5 (i, __VA_ARGS__), _f[i]
#define YT_PRI_FCALL_ARGS_ARRAY_5(i, t, ...)  YT_PRI_FCALL_ARGS_ARRAY_4 (i, __VA_ARGS__), _e[i]
#define YT_PRI_FCALL_ARGS_ARRAY_4(i, t, ...)  YT_PRI_FCALL_ARGS_ARRAY_3 (i, __VA_ARGS__), _d[i]
#define YT_PRI_FCALL_ARGS_ARRAY_3(i, t, ...)  YT_PRI_FCALL_ARGS_ARRAY_2 (i, __VA_ARGS__), _c[i]
#define YT_PRI_FCALL_ARGS_ARRAY_2(i, t, ...)  YT_PRI_FCALL_ARGS_ARRAY_1 (i, __VA_ARGS__), _b[i]
#define YT_PRI_FCALL_ARGS_ARRAY_1(i, t, ...)  _a[i]
#define YT_PRI_FCALL_ARGS_ARRAY_0(...)

#define YT_PRI_FUNC_PARAMS_X(n, ...)  YT_PRI_FUNC_PARAMS_##n (__VA_ARGS__)
#define YT_PRI_FUNC_PARAMS_10(t, ...) YT_PRI_FUNC_PARAMS_9 (__VA_ARGS__), t _j
#define YT_PRI_FUNC_PARAMS_9(t, ...)  YT_PRI_FUNC_PARAMS_8 (__VA_ARGS__), t _i
#define YT_PRI_FUNC_PARAMS_8(t, ...)  YT_PRI_FUNC_PARAMS_7 (__VA_ARGS__), t _h
#define YT_PRI_FUNC_PARAMS_7(t, ...)  YT_PRI_FUNC_PARAMS_6 (__VA_ARGS__), t _g
#define YT_PRI_FUNC_PARAMS_6(t, ...)  YT_PRI_FUNC_PARAMS_5 (__VA_ARGS__), t _f
#define YT_PRI_FUNC_PARAMS_5(t, ...)  YT_PRI_FUNC_PARAMS_4 (__VA_ARGS__), t _e
#define YT_PRI_FUNC_PARAMS_4(t, ...)  YT_PRI_FUNC_PARAMS_3 (__VA_ARGS__), t _d
#define YT_PRI_FUNC_PARAMS_3(t, ...)  YT_PRI_FUNC_PARAMS_2 (__VA_ARGS__), t _c
#define YT_PRI_FUNC_PARAMS_2(t, ...)  YT_PRI_FUNC_PARAMS_1 (__VA_ARGS__), t _b
#define YT_PRI_FUNC_PARAMS_1(t, ...)  t _a
#define YT_PRI_FUNC_PARAMS_0(...)

#define YT_PRI_FUNC_PARAMS_ARRAY_X(n, ...)  YT_PRI_FUNC_PARAMS_ARRAY_##n (__VA_ARGS__)
#define YT_PRI_FUNC_PARAMS_ARRAY_10(t, ...) YT_PRI_FUNC_PARAMS_ARRAY_9 (__VA_ARGS__), t _j[]
#define YT_PRI_FUNC_PARAMS_ARRAY_9(t, ...)  YT_PRI_FUNC_PARAMS_ARRAY_8 (__VA_ARGS__), t _i[]
#define YT_PRI_FUNC_PARAMS_ARRAY_8(t, ...)  YT_PRI_FUNC_PARAMS_ARRAY_7 (__VA_ARGS__), t _h[]
#define YT_PRI_FUNC_PARAMS_ARRAY_7(t, ...)  YT_PRI_FUNC_PARAMS_ARRAY_6 (__VA_ARGS__), t _g[]
#define YT_PRI_FUNC_PARAMS_ARRAY_6(t, ...)  YT_PRI_FUNC_PARAMS_ARRAY_5 (__VA_ARGS__), t _f[]
#define YT_PRI_FUNC_PARAMS_ARRAY_5(t, ...)  YT_PRI_FUNC_PARAMS_ARRAY_4 (__VA_ARGS__), t _e[]
#define YT_PRI_FUNC_PARAMS_ARRAY_4(t, ...)  YT_PRI_FUNC_PARAMS_ARRAY_3 (__VA_ARGS__), t _d[]
#define YT_PRI_FUNC_PARAMS_ARRAY_3(t, ...)  YT_PRI_FUNC_PARAMS_ARRAY_2 (__VA_ARGS__), t _c[]
#define YT_PRI_FUNC_PARAMS_ARRAY_2(t, ...)  YT_PRI_FUNC_PARAMS_ARRAY_1 (__VA_ARGS__), t _b[]
#define YT_PRI_FUNC_PARAMS_ARRAY_1(t, ...)  t _a[]
#define YT_PRI_FUNC_PARAMS_ARRAY_0(...)
/*
 * ========================================================================================
 * SECTION 1: FOR CREATING MOCK FUNCTION DECLARATION & DEFINITIONS
 * ========================================================================================
 * 1.1: RECORD CALL MACROS
 * ========================================================================================
 * */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#ifndef YUKTI_TEST_NO_MUST_CALL

void yt_pri_add_callrecord (ACL_ListNode* head, int sourceLineNumber,
                            const char* const sourceFileName, int n, const char* const fn, ...);

extern ACL_ListNode yt_pri_actualCallListHead;

typedef struct YT_PRI_Arg {
    bool isOpt;
    uintptr_t val; // A type large enough to hold both integers & addresses
} YT_PRI_Arg;

    #define YT_PRI_RECORD_CALL_X(n, ...) YT_PRI_FCALL_WRAP_ARGS_X (n, YT_V, ##__VA_ARGS__)

    #define YT_V(v)                               \
        (YT_PRI_Arg)                              \
        {                                         \
            .isOpt = false, .val = (uintptr_t)(v) \
        }
    #define _                       \
        (YT_PRI_Arg)                \
        {                           \
            .isOpt = true, .val = 0 \
        }

    #define YT_PRI_RECORD_CALL(n, f, ...)                                                     \
        do {                                                                                  \
            yt_pri_add_callrecord (&yt_pri_actualCallListHead, __LINE__, __FILE__,            \
                                   YT_PRI_COUNT_ARGS (__VA_ARGS__),                           \
                                   #f __VA_OPT__ (, ) YT_PRI_RECORD_CALL_X (n, __VA_ARGS__)); \
        } while (0)
#else
    #define YT_PRI_RECORD_CALL(...) (void)0
#endif /* YUKTI_TEST_NO_MUST_CALL */

/*
 * ========================================================================================
 * SECTION 1: FOR CREATING MOCK FUNCTION DECLARATION & DEFINITIONS
 * ========================================================================================
 * 1.2: MOCKS TO CREATE FAKE/MOCK FUNCTION DEFINITIONS & DECLARATIONS
 * ========================================================================================
 * */
void reset(); // MUST BE DEFINED BY THE USER OF THIS HEADER FILE.

// ----------------------------------------------------------------------------
// Common Helper macros used by both declaration and defination macros.
// ----------------------------------------------------------------------------
#define YT_PRI_STRUCT_TAG(f)     f##_fake_tag
#define YT_PRI_STRUCT_VAR(f)     f##_fake
#define YT_PRI_STRUCT_HANDLER(f) f##_fake_handler

// ---------------------[ FAKE FUNCTION DEFINITION ]-----------------------------
#define YT_DEFINE_FUNC_VOID(f, ...) \
    YT_PRI_DEFINE_FUNC_STRUCT (f);  \
    YT_PRI_DEFINE_FUNC_BODY_VOID (YT_PRI_COUNT_ARGS (__VA_ARGS__), f, __VA_ARGS__)

// ----
#define YT_DEFINE_FUNC(rt, f, ...) \
    YT_PRI_DEFINE_FUNC_STRUCT (f); \
    YT_PRI_DEFINE_FUNC_BODY (YT_PRI_COUNT_ARGS (__VA_ARGS__), rt, f, __VA_ARGS__)

// ----
#define YT_PRI_DEFINE_FUNC_STRUCT(f) YT_PRI_STRUCT_TAG (f) YT_PRI_STRUCT_VAR (f) = { 0 }

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
    YT_PRI_STRUCT_VAR (f).handler (YT_PRI_FCALL_ARGS_##n())

#define YT_PRI_RETURN(n, f, ...)                                         \
    return (YT_PRI_STRUCT_VAR (f).handler)                               \
               ? YT_PRI_STRUCT_VAR (f).handler (YT_PRI_FCALL_ARGS_##n()) \
               : YT_PRI_STRUCT_VAR (f).ret

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
#define YT_PRI_STRUCT_FIELD(pt, p)     pt p
#define YT_PRI_DECLARE_STRUCT_END(f) \
    }                                \
    YT_PRI_STRUCT_TAG (f);           \
    extern YT_PRI_STRUCT_TAG (f) YT_PRI_STRUCT_VAR (f)

// -----------------------[ RESET MOCK ]------------------------
#define YT_RESET_MOCK(f) memset (&YT_PRI_STRUCT_VAR (f), 0, sizeof (YT_PRI_STRUCT_VAR (f)))

/*
 * ========================================================================================
 * SECTION 2: FOR USAGE IN TEST IMPLEMENTATION TO VALIDATE TEST EXPECTATIONS & REPORTING
 * ========================================================================================
 * 2.1: FOR REPORTING LIST OF FALIED TESTS IN THE END.
 * ========================================================================================
 * */
#ifdef YUKTI_TEST_IMPLEMENTATION

    #define YT_PRI_MAX_TEST_FUNCTION_NAME_LENGTH 250
    #define YT_PRI_EXIT_FAILURE                  1
    #define YT_PRI_EXIT_SUCCESS                  0

typedef struct YT_PRI_TestRecord {
    char test_function_name[YT_PRI_MAX_TEST_FUNCTION_NAME_LENGTH];
    uint32_t total_exp_count;
    uint32_t failed_exp_count;
    ACL_ListNode failedTestListNode;
} YT_PRI_TestRecord;

static ACL_ListNode yt_pri_failedTestsListHead;
static YT_PRI_TestRecord* yt_pri_current_testrecord = NULL;
static uint32_t yt_pri_total_test_count             = 0;
static uint32_t yt_pri_failed_test_count            = 0;

static YT_PRI_TestRecord* yt_pri_create_testRecord (char* testname)
{
    assert (testname != NULL);

    YT_PRI_TestRecord* newrec = NULL;
    if (!(newrec = calloc (1, sizeof (YT_PRI_TestRecord)))) {
        perror ("malloc");
        YT_PRI_PANIC (NULL);
    }

    newrec->total_exp_count  = 0;
    newrec->failed_exp_count = 0;
    strncpy (newrec->test_function_name, testname, YT_PRI_MAX_TEST_FUNCTION_NAME_LENGTH - 1);
    acl_list_init (&newrec->failedTestListNode);

    return newrec;
}

static void yt_pri_free_testRecord (YT_PRI_TestRecord* trecord)
{
    // Expectation: Input pointers are not NULL. They are not user facing!
    assert (trecord != NULL);
    free (trecord);
}

    #define YT_INIT()                                    \
        do {                                             \
            acl_list_init (&yt_pri_failedTestsListHead); \
            yt_pri_total_test_count = 0;                 \
        } while (0)

    #define YT_RETURN_WITH_REPORT()                                                           \
        do {                                                                                  \
            printf ("\n%s[ Tests Summary ]%s", YT_PRI_COL_HIGHLIGHT, YT_PRI_COL_RESET);       \
            if (yt_pri_failed_test_count == 0) {                                              \
                printf ("\n  %sAll tests passed [0 of %d failed]%s\n", YT_PRI_COL_GREEN,      \
                        yt_pri_total_test_count, YT_PRI_COL_RESET);                           \
            } else {                                                                          \
                printf ("\n%sNot all tests passed [%d of %d failed]%s", YT_PRI_COL_RED,       \
                        yt_pri_failed_test_count, yt_pri_total_test_count, YT_PRI_COL_RESET); \
                ACL_ListNode* node;                                                           \
                acl_list_for_each (&yt_pri_failedTestsListHead, node)                         \
                {                                                                             \
                    YT_PRI_TestRecord* test = ACL_LIST_ITEM (node, YT_PRI_TestRecord,         \
                                                             failedTestListNode);             \
                    printf ("\n    %s* '%s' test failed%s", YT_PRI_COL_RED,                   \
                            test->test_function_name, YT_PRI_COL_RESET);                      \
                }                                                                             \
            }                                                                                 \
            printf ("\n");                                                                    \
            return yt_pri_failed_test_count == 0 ? YT_PRI_EXIT_SUCCESS : YT_PRI_EXIT_FAILURE; \
        } while (0)

    /*
     * ========================================================================================
     * SECTION 2: FOR USAGE IN TEST IMPLEMENTATION TO VALIDATE TEST EXPECTATIONS & REPORTING
     * ========================================================================================
     * 2.2: FUNCTION & MACROS TO TEST EXPECTATIONS ON FUNCTION CALLS
     * ========================================================================================
     * */
    #ifdef YUKTI_TEST_NO_MUST_CALL
        // Compilation will fail since the these macros will expand to invalid C code.
        #define YT_PRI_ERROR_MESSAGE Invalid when YUKTI_TEST_NO_MUST_CALL is defined

        #define YT_MUST_CALL_IN_ORDER(...)                YT_PRI_ERROR_MESSAGE
        #define YT_MUST_CALL_IN_ORDER_ATLEAST_TIMES(...)  YT_PRI_ERROR_MESSAGE
        #define YT_MUST_CALL_ANY_ORDER(...)               YT_PRI_ERROR_MESSAGE
        #define YT_MUST_CALL_ANY_ORDER_ATLEAST_TIMES(...) YT_PRI_ERROR_MESSAGE

        #define yt_pri_validate_expectations() (void)0
        #define yt_pri_teardown()              (void)0
        #define yt_pri_ec_init()               (void)0
    #else
        #define YT_MUST_NEVER_CALL(f, ...)                                                        \
            do {                                                                                  \
                yt_pri_current_testrecord->total_exp_count++;                                     \
                yt_pri_add_callrecord (&yt_pri_neverCallExceptationsListHead, __LINE__, __FILE__, \
                                       YT_PRI_COUNT_ARGS (__VA_ARGS__) / 2, #f, ##__VA_ARGS__);   \
            } while (0)

        #define YT_MUST_CALL_IN_ORDER(f, ...)                                                   \
            do {                                                                                \
                yt_pri_current_testrecord->total_exp_count++;                                   \
                yt_pri_add_callrecord (&yt_pri_orderedExceptationListHead, __LINE__, __FILE__,  \
                                       YT_PRI_COUNT_ARGS (__VA_ARGS__) / 2, #f, ##__VA_ARGS__); \
            } while (0)

        #define YT_MUST_CALL_IN_ORDER_ATLEAST_TIMES(n, f, ...) \
            for (int i = n; i; i--) {                          \
                YT_MUST_CALL_IN_ORDER (f, ##__VA_ARGS__);      \
            }

        #define YT_MUST_CALL_ANY_ORDER(f, ...)                                                  \
            do {                                                                                \
                yt_pri_current_testrecord->total_exp_count++;                                   \
                yt_pri_add_callrecord (&yt_pri_globalExceptationListHead, __LINE__, __FILE__,   \
                                       YT_PRI_COUNT_ARGS (__VA_ARGS__) / 2, #f, ##__VA_ARGS__); \
            } while (0)

        #define YT_MUST_CALL_ANY_ORDER_ATLEAST_TIMES(n, f, ...) \
            for (int i = n; i; i--) {                           \
                YT_MUST_CALL_ANY_ORDER (f, ##__VA_ARGS__);      \
            }

        #define YT_PRI_MAX_CALLSTRING_SIZE      250
        #define YT_PRI_MAX_SOURCE_FILE_NAME_LEN 250
        #define YT_PRI_ARG_OPTIONAL_CHAR        '!'
        #define YT_PRI_ARG_SEPARATOR_CHAR       ','

typedef struct YT_PRI_CallRecord {
    char callString[YT_PRI_MAX_CALLSTRING_SIZE];
    enum {
        YT_CALLRECORD_TYPE_ORDERED_EXPECTATION,
        YT_CALLRECORD_TYPE_GLOBAL_EXPECTATION,
        YT_CALLRECORD_TYPE_ACTUALCALL,
        YT_CALLRECORD_TYPE_NEVER_CALL_EXPECTATION,
    } type;
    int sourceLineNumber;
    char* sourceFileName;
    ACL_ListNode listNode;
} YT_PRI_CallRecord;

static bool yt_pri_match_call_strings (const char* exp, const char* actual);
static void yt_pri_string_append (char* str, size_t size, const char* const fmt, ...);
static void yt_pri_call_record_free (YT_PRI_CallRecord* node);
static void yt_pri_free_call_list (ACL_ListNode* head);
        #ifdef YUKTI_TEST_DEBUG
static void yt_pri_create_call_string (ACL_ListNode* head, char* buffer, size_t buffer_size, int n,
                                       const char* const fn, va_list l);
        #else
static void yt_pri_create_call_string (char* buffer, size_t buffer_size, int n,
                                       const char* const fn, va_list l);
        #endif /* YUKTI_TEST_DEBUG */

static void yt_pri_print_unmet_expectations();
static void yt_pri_validate_expectations();
static void yt_pri_ec_init();
static void yt_pri_teardown();

static ACL_ListNode yt_pri_neverCallExceptationsListHead;
static ACL_ListNode yt_pri_orderedExceptationListHead;
static ACL_ListNode yt_pri_globalExceptationListHead;
ACL_ListNode yt_pri_actualCallListHead;

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

    size_t len = strlen (str);

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

static void yt_pri_free_call_list (ACL_ListNode* head)
{
    // Expectation: Input pointers are not NULL. They are not user facing!
    assert (head != NULL);

    while (!acl_list_is_empty (head)) {
        YT_PRI_CallRecord* item = ACL_LIST_ITEM (head->next, YT_PRI_CallRecord, listNode);
        yt_pri_call_record_free (item);
    }
}

static void yt_pri_call_record_free (YT_PRI_CallRecord* node)
{
    // Expectation: Input pointers are not NULL. They are not user facing!
    assert (node != NULL);

    acl_list_remove (&node->listNode);
    free (node->sourceFileName);
    free (node);
}

        #ifdef YUKTI_TEST_DEBUG
void yt_pri_create_call_string (ACL_ListNode* head, char* buffer, size_t buffer_size, int n,
                                const char* const fn, va_list l)
        #else
void yt_pri_create_call_string (char* buffer, size_t buffer_size, int n, const char* const fn,
                                va_list l)
        #endif /* YUKTI_TEST_DEBUG */
{
    // Expectation: Input pointers are not NULL and Buffer size > 0. They are not user facing!
    assert (buffer != NULL && fn != NULL && buffer_size > 0);

    yt_pri_string_append (buffer, buffer_size, "%s(", fn);

    for (int i = 0; i < n; i++) {
        YT_PRI_Arg item = va_arg (l, YT_PRI_Arg);
        char separator  = (i == 0) ? ' ' : YT_PRI_ARG_SEPARATOR_CHAR;

        if (item.isOpt) {
        #ifdef YUKTI_TEST_DEBUG
            // Expectation: Actual call list must not have optional arguments
            assert (head != &yt_pri_actualCallListHead);
        #endif /* YUKTI_TEST_DEBUG */
            yt_pri_string_append (buffer, buffer_size, "%c%c", separator, YT_PRI_ARG_OPTIONAL_CHAR);
        } else {
            yt_pri_string_append (buffer, buffer_size, "%c%d", separator, item.val);
        }
    }
    yt_pri_string_append (buffer, buffer_size, ")");
    va_end (l);
}

void yt_pri_add_callrecord (ACL_ListNode* head, int sourceLineNumber,
                            const char* const sourceFileName, int n, const char* const fn, ...)
{
    YT_PRI_CallRecord* newrec = NULL;
    if (!(newrec = malloc (sizeof (YT_PRI_CallRecord)))) {
        perror ("malloc");
        YT_PRI_PANIC (NULL);
    }
    if (!(newrec->sourceFileName = malloc (sizeof (char) * YT_PRI_MAX_SOURCE_FILE_NAME_LEN))) {
        perror ("malloc");
        YT_PRI_PANIC (NULL);
    }

    if (head == &yt_pri_orderedExceptationListHead) {
        newrec->type = YT_CALLRECORD_TYPE_ORDERED_EXPECTATION;
    } else if (head == &yt_pri_globalExceptationListHead) {
        newrec->type = YT_CALLRECORD_TYPE_GLOBAL_EXPECTATION;
    } else if (head == &yt_pri_actualCallListHead) {
        newrec->type = YT_CALLRECORD_TYPE_ACTUALCALL;
    } else if (head == &yt_pri_neverCallExceptationsListHead) {
        newrec->type = YT_CALLRECORD_TYPE_NEVER_CALL_EXPECTATION;
    } else {
        YT_PRI_PANIC ("Invalid list");
    }

    acl_list_init (&newrec->listNode);
    acl_list_add_before (head, &newrec->listNode);
    newrec->callString[0]    = '\0';
    newrec->sourceLineNumber = sourceLineNumber;
    strncpy (newrec->sourceFileName, sourceFileName, YT_PRI_MAX_SOURCE_FILE_NAME_LEN - 1);

    va_list l;
    va_start (l, fn);
        #ifdef YUKTI_TEST_DEBUG
    yt_pri_create_call_string (head, newrec->callString, sizeof (newrec->callString), n, fn, l);
        #else
    yt_pri_create_call_string (newrec->callString, sizeof (newrec->callString), n, fn, l);
        #endif /* YUKTI_TEST_DEBUG */
    va_end (l);
}

void yt_pri_print_unmet_expectations (ACL_ListNode* neverCallExpectationFailedListHead)
{
    ACL_ListNode* node;
    if (!acl_list_is_empty (neverCallExpectationFailedListHead)) {
        acl_list_for_each (neverCallExpectationFailedListHead, node)
        {
            YT_PRI_CallRecord* item = ACL_LIST_ITEM (node, YT_PRI_CallRecord, listNode);

            // Never call List must contain only call records of never call expectations
            assert (item->type == YT_CALLRECORD_TYPE_NEVER_CALL_EXPECTATION);

            YT_PRI_FAILED (Expectation not met,
                           "Called, when should be never called: %s\n\tAt: %s:%d", item->callString,
                           item->sourceFileName, item->sourceLineNumber);
        }
    }

    if (!acl_list_is_empty (&yt_pri_globalExceptationListHead)) {
        acl_list_for_each (&yt_pri_globalExceptationListHead, node)
        {
            YT_PRI_CallRecord* item = ACL_LIST_ITEM (node, YT_PRI_CallRecord, listNode);

            // Global List must contain only call records of global/unordered call expectations
            assert (item->type == YT_CALLRECORD_TYPE_GLOBAL_EXPECTATION);

            YT_PRI_FAILED (Expectation not met, "Never called: %s\n\tAt: %s:%d", item->callString,
                           item->sourceFileName, item->sourceLineNumber);
        }
    }
    if (!acl_list_is_empty (&yt_pri_orderedExceptationListHead)) {
        acl_list_for_each (&yt_pri_orderedExceptationListHead, node)
        {
            YT_PRI_CallRecord* item = ACL_LIST_ITEM (node, YT_PRI_CallRecord, listNode);

            // Ordered List must contain only call records of Ordered call expectations
            assert (item->type == YT_CALLRECORD_TYPE_ORDERED_EXPECTATION);

            YT_PRI_FAILED (Expectation not met, "Never called/called out of order: %s\n\tAt %s:%d",
                           item->callString, item->sourceFileName, item->sourceLineNumber);
        }
    }

        #ifdef YUKTI_TEST_DEBUG
    printf ("\n  Actual order of functions calls was the following:\n");
    acl_list_for_each (&yt_pri_actualCallListHead, node)
    {
        YT_PRI_CallRecord* item = ACL_LIST_ITEM (node, YT_PRI_CallRecord, listNode);

        // Actual List must contain only call records of actual calls
        assert (item->type == YT_CALLRECORD_TYPE_ACTUALCALL);

        printf ("    * %s\n", item->callString);
    }
        #endif /* YUKTI_TEST_DEBUG */
}

void yt_pri_validate_expectations()
{
    ACL_ListNode* actCallNode;

    ACL_ListNode neverCallExpectationFailedListHead;
    acl_list_init (&neverCallExpectationFailedListHead);

    acl_list_for_each (&yt_pri_actualCallListHead, actCallNode)
    {
        YT_PRI_CallRecord* item = ACL_LIST_ITEM (actCallNode, YT_PRI_CallRecord, listNode);

        // Actual List must contain only call records of actual calls
        assert (item->type == YT_CALLRECORD_TYPE_ACTUALCALL);

        ACL_ListNode* neverCallNode;
        acl_list_for_each (&yt_pri_neverCallExceptationsListHead, neverCallNode)
        {
            YT_PRI_CallRecord* neverExp = ACL_LIST_ITEM (neverCallNode, YT_PRI_CallRecord,
                                                         listNode);

            // Never call List must contain only call records of never call expectations
            assert (neverExp->type == YT_CALLRECORD_TYPE_NEVER_CALL_EXPECTATION);

            if (yt_pri_match_call_strings (neverExp->callString, item->callString)) {
                acl_list_remove (&neverExp->listNode);
                acl_list_add_before (&neverCallExpectationFailedListHead, &neverExp->listNode);
                break;
            }
        }

        ACL_ListNode* globalCallNode;
        bool globalExpectationMet = false;
        acl_list_for_each (&yt_pri_globalExceptationListHead, globalCallNode)
        {
            YT_PRI_CallRecord* gloExp = ACL_LIST_ITEM (globalCallNode, YT_PRI_CallRecord, listNode);

            // Global List must contain only call records of global/unordered call expectations
            assert (gloExp->type == YT_CALLRECORD_TYPE_GLOBAL_EXPECTATION);

            if (yt_pri_match_call_strings (gloExp->callString, item->callString)) {
                globalExpectationMet = true;
                yt_pri_call_record_free (gloExp);
                break;
            }
        }

        if (!globalExpectationMet && !acl_list_is_empty (&yt_pri_orderedExceptationListHead)) {
            YT_PRI_CallRecord* ordExp = ACL_LIST_ITEM (yt_pri_orderedExceptationListHead.next,
                                                       YT_PRI_CallRecord, listNode);

            // Ordered List must contain only call records of Ordered call expectations
            assert (ordExp->type == YT_CALLRECORD_TYPE_ORDERED_EXPECTATION);

            if (yt_pri_match_call_strings (ordExp->callString, item->callString)) {
                yt_pri_call_record_free (ordExp);
            }
        }
    }

    bool success = acl_list_is_empty (&yt_pri_orderedExceptationListHead) &&
                   acl_list_is_empty (&yt_pri_globalExceptationListHead) &&
                   acl_list_is_empty (&neverCallExpectationFailedListHead);

    if (!success) {
        yt_pri_print_unmet_expectations (&neverCallExpectationFailedListHead);
        yt_pri_free_call_list (&neverCallExpectationFailedListHead);
    }
}

static void yt_pri_teardown()
{
    yt_pri_free_call_list (&yt_pri_neverCallExceptationsListHead);
    yt_pri_free_call_list (&yt_pri_globalExceptationListHead);
    yt_pri_free_call_list (&yt_pri_orderedExceptationListHead);
    yt_pri_free_call_list (&yt_pri_actualCallListHead);
}

static void yt_pri_ec_init()
{
    acl_list_init (&yt_pri_neverCallExceptationsListHead);
    acl_list_init (&yt_pri_globalExceptationListHead);
    acl_list_init (&yt_pri_orderedExceptationListHead);
    acl_list_init (&yt_pri_actualCallListHead);
}
    #endif     /* YUKTI_TEST_NO_MUST_CALL */
#endif         /* YUKTI_TEST_IMPLEMENTATION */

/*
 * ========================================================================================
 * SECTION 2: FOR USAGE IN TEST IMPLEMENTATION TO VALIDATE TEST EXPECTATIONS & REPORTING
 * ========================================================================================
 * 2.3: FUNCTION & MACROS TO TEST STATE EXPECTATIONS
 * ========================================================================================
 * */
#ifdef YUKTI_TEST_IMPLEMENTATION
static int yt_pri_equal_mem (const void* a, const void* b, unsigned long size, int* i);
static int yt_pri_equal_string (const char* a, const char* b, int* i);

    #define YT_PRI_TEST_SCALAR(a, o, b)                               \
        do {                                                          \
            __auto_type ut_a = (a);                                   \
            __auto_type ut_b = (b);                                   \
            yt_pri_current_testrecord->total_exp_count++;             \
            if (ut_a o ut_b)                                          \
                YT_PRI_PASSED (a o b);                                \
            else                                                      \
                YT_PRI_FAILED (a o b, "[%d !" #o " %d]", ut_a, ut_b); \
        } while (0)

    #define YT_PRI_TEST_MEM(a, o, b, sz)                                                    \
        do {                                                                                \
            __auto_type ut_a = (a);                                                         \
            __auto_type ut_b = (b);                                                         \
            yt_pri_current_testrecord->total_exp_count++;                                   \
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
            yt_pri_current_testrecord->total_exp_count++;                                   \
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

    /*
     * ========================================================================================
     * SECTION 2: FOR USAGE IN TEST IMPLEMENTATION TO VALIDATE TEST EXPECTATIONS & REPORTING
     * ========================================================================================
     * 2.4: FUNCTION & MACROS TO PARAMETERISED TEST IMPLEMENTATION
     * ========================================================================================
     * */
    #define YT_PRI_TEST_IMPL_BODY(tf, fn, ...)                                      \
        reset();                                                                    \
        yt_pri_ec_init();                                                           \
        yt_pri_total_test_count++;                                                  \
        /* Following assert ensures we are not overriding it. It was taken cared of \
         * in the previous test's YT_END. */                                        \
        assert (yt_pri_current_testrecord == NULL);                                 \
        yt_pri_current_testrecord = yt_pri_create_testRecord (#fn);                 \
        do

    #define YT_A(t) (t[])

    #define YT_PRI_TESTP_DECLARE_TEST_FUNC(fn, ...) \
        static void yt_pri_##fn##_test (size_t, size_t, __VA_ARGS__)

    #define YT_PRI_TESTP_DEFINE_TEST_FUNC(n, tf, fn, ...)                                       \
        static void yt_pri_##fn##_test (size_t count, size_t i,                                 \
                                        YT_PRI_FUNC_PARAMS_X (n, __VA_ARGS__))                  \
        {                                                                                       \
            printf ("%s[%20s - %-20s](%lu of %lu)%s", YT_PRI_COL_HIGHLIGHT, #tf, #fn, i, count, \
                    YT_PRI_COL_RESET);                                                          \
            YT_PRI_TEST_IMPL_BODY (tf, fn)

    #define YT_PRI_TESTP_DEFINE_TEST_WRAPPER_FUNC(n, fn, ...)                                 \
        static void fn (size_t count, YT_PRI_FUNC_PARAMS_ARRAY_X (n, __VA_ARGS__))            \
        {                                                                                     \
            for (unsigned i = 0; i < count; i++) {                                            \
                yt_pri_##fn##_test (count, i, YT_PRI_FCALL_ARGS_ARRAY_X (n, i, __VA_ARGS__)); \
            }                                                                                 \
        }

    #define YT_TESTP(tf, fn, ...)                                                                \
        YT_PRI_TESTP_DECLARE_TEST_FUNC (fn, __VA_ARGS__);                                        \
        YT_PRI_TESTP_DEFINE_TEST_WRAPPER_FUNC (YT_PRI_COUNT_ARGS (__VA_ARGS__), fn, __VA_ARGS__) \
        YT_PRI_TESTP_DEFINE_TEST_FUNC (YT_PRI_COUNT_ARGS (__VA_ARGS__), tf, fn, __VA_ARGS__)

    #define YT_TEST(tf, fn)                                                                  \
        static void fn()                                                                     \
        {                                                                                    \
            printf ("%s[%20s - %-20s]%s", YT_PRI_COL_HIGHLIGHT, #tf, #fn, YT_PRI_COL_RESET); \
            YT_PRI_TEST_IMPL_BODY (tf, fn)

    // clang-format off
    #define YT_END()                                                              \
        yt_pri_validate_expectations();                                           \
        yt_pri_teardown();                                                        \
        if (yt_pri_current_testrecord->failed_exp_count != 0) {                   \
            /* Add test to failed test list */                                    \
            printf ("\n  %sSome test expectations failed [%d of %d failed]%s",    \
                    YT_PRI_COL_RED, yt_pri_current_testrecord->failed_exp_count,  \
                    yt_pri_current_testrecord->total_exp_count, YT_PRI_COL_RESET);\
            yt_pri_failed_test_count++;                                           \
            acl_list_add_before (&yt_pri_failedTestsListHead,                     \
                                 &yt_pri_current_testrecord->failedTestListNode); \
        } else {                                                                  \
            printf ("\n  %sAll test expectations passed [0 of %d failed]%s",      \
                    YT_PRI_COL_GREEN, yt_pri_current_testrecord->total_exp_count, \
                    YT_PRI_COL_RESET);                                            \
            yt_pri_free_testRecord (yt_pri_current_testrecord);                   \
        }                                                                         \
        yt_pri_current_testrecord = NULL;                                         \
        /* following '}' is for closing YT_TEST's do loop */                      \
        } while (0);                                                              \
        printf ("\n");
    // clang-format on

#endif /* YUKTI_TEST_IMPLEMENTATION */

#pragma GCC diagnostic pop
#endif // YUKTI_TEST_H
