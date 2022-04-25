/*
 * ------------------------------------------------------------------------------------------------
 * Fake function macros for Unittesting parts of a program.
 * These macros make it easy to create fake/mock functions, with support for return and byref
 * parameters.
 *
 * Author: arjobmukherjee@gmail.com (Arjob Mukherjee)
 * Dated : 26 April 2022
 * ------------------------------------------------------------------------------------------------
 * Examples:
 *
 * Description                 C declaration              Fake declaration
 * -------------------------   ------------------------   -----------------------------------------
 * no param input              void f ();                 DECLARE_FUNC_VOID_0(f);
 * one input param int         void f (int a);            DECLARE_FUNC_VOID_1(f, IN, int);
 * one input param int ptr     void f (int *a);           DECLARE_FUNC_VOID_1(f, IN, int*);
 * one byref param int ptr     void f (int *a);           DECLARE_FUNC_VOID_1(f, OUT, int*);
 * bytef out and input params  void f (int a, char* b);   DECLARE_FUNC_VOID_1(f, IN, int, 
 *                                                                               OUT, * char*);
 * no param input              int f ();                  DECLARE_FUNC_VOID_0(int, f);
 * one input param int         int f (int a);             DECLARE_FUNC_VOID_1(int, f, IN, int);
 * one input param int ptr     int f (int *a);            DECLARE_FUNC_VOID_1(int, f, IN, int*);
 * one byref param int ptr     int f (int *a);            DECLARE_FUNC_VOID_1(int, f, OUT, int*);
 * bytef out and input params  int f (int a, char* b);    DECLARE_FUNC_VOID_1(int, f, IN, int, 
 *                                                                            OUT, * char*);
 * ------------------------------------------------------------------------------------------------
 */

#ifndef UNITTESTS_FAKE
#define UNITTESTS_FAKE

/*
 * -------------------------------------
 * Macro param nomenclature
 * Param   Meaning
 * ------  -----------------------------
 * f       function name
 * pt      param type
 * pd      param direction (input/output)
 * p       parameter name
 * rt      FK_RETURN type
 * -------------------------------------
 */

// ---------------------------------------------------------------------------------------
// Common Helper macros used by both declaration and defination macros.
// ---------------------------------------------------------------------------------------
#define FAKE_STRUCT_TAG(f) f ## _fake_t
#define FAKE_STRUCT_VAR(f) f ## _fake
#define OUT   OUT
#define IN    IN

// =============================[ FAKE FUNCTION DEFINATION ]======================================
// ---------------------------------------------------------------------------------------
// Macros to craete fake function declarations
// ---------------------------------------------------------------------------------------
#define FK_DEFINE_FUNC_BODY_0(rt, f)           rt f() 
#define FK_DEFINE_FUNC_BODY_1(rt, f, pt1)      rt f(pt1 a) 
#define FK_DEFINE_FUNC_BODY_2(rt, f, pt1, pt2) rt f(pt1 a, pt2 b)
// ---------------------------------------------------------------------------------------
// Helper Macros
// ---------------------------------------------------------------------------------------
#define FK_DEFINE_FUNC_STRUCT(f)      FAKE_STRUCT_TAG(f) FAKE_STRUCT_VAR(f) = {}
#define FK_INCREMENT_INVOKE_COUNT(f)  FAKE_STRUCT_VAR(f).invoke_count++;
// ------
#define FK_PARAM_OUT_SET(f, p) *p = FAKE_STRUCT_VAR(f).p[FAKE_STRUCT_VAR(f).invoke_count]
#define FK_PARAM_IN_SET(f, p)  /* No need for a field for input param */

#define FK_PARAM_SET(f, pd, p) FK_PARAM_ ## pd ## _SET(f, p)
// ------
#define FK_RETURN(f) return FAKE_STRUCT_VAR(f).ret[FAKE_STRUCT_VAR(f).invoke_count - 1]
// ---------------------------------------------------------------------------------------
// Complete fake function definations
// ---------------------------------------------------------------------------------------
#define DEFINE_FUNC_VOID_0(f)                    FK_DEFINE_FUNC_STRUCT(f);          \
                                                 FK_DEFINE_FUNC_BODY_0(void, f) {   \
                                                     FK_INCREMENT_INVOKE_COUNT(f); \
                                                 }
// ------
#define DEFINE_FUNC_VOID_1(f, pd1, pt1)          FK_DEFINE_FUNC_STRUCT(f);               \
                                                 FK_DEFINE_FUNC_BODY_1(void, f, pt1) {   \
                                                     FK_PARAM_SET(f, pd1, a);            \
                                                     FK_INCREMENT_INVOKE_COUNT(f);       \
                                                  }
// ------
#define DEFINE_FUNC_VOID_2(f, pd1, pt1, pd2, pt2) FK_DEFINE_FUNC_STRUCT(f);                   \
                                                  FK_DEFINE_FUNC_BODY_2(void, f, pt1, pt2) {  \
                                                      FK_PARAM_SET(f, pd1, a);                \
                                                      FK_PARAM_SET(f, pd2, b);                \
                                                      FK_INCREMENT_INVOKE_COUNT(f);           \
                                                  }
// ---------------------------------------------------------------------------------------
#define DEFINE_FUNC_0(rt, f)                   FK_DEFINE_FUNC_STRUCT(f);         \
                                               FK_DEFINE_FUNC_BODY_0(rt, f) {    \
                                                   FK_INCREMENT_INVOKE_COUNT(f); \
                                                   FK_RETURN(f);                 \
                                               }
// ------
#define DEFINE_FUNC_1(rt, f, pd1, pt1)         FK_DEFINE_FUNC_STRUCT(f);               \
                                               FK_DEFINE_FUNC_BODY_1(rt, f, pt1) {     \
                                                   FK_PARAM_SET(f, pd1, a);            \
                                                   FK_INCREMENT_INVOKE_COUNT(f);       \
                                                   FK_RETURN(f);                       \
                                               }
// ------
#define DEFINE_FUNC_(rt, f, pd1, pt1, pd2, pt2) FK_DEFINE_FUNC_STRUCT(f);                   \
                                                FK_DEFINE_FUNC_BODY_2(void, f, pt1, pt2) {  \
                                                    FK_PARAM_SET(f, pd1, a);                \
                                                    FK_PARAM_SET(f, pd2, b);                \
                                                    FK_INCREMENT_INVOKE_COUNT(f);           \
                                                    FK_RETURN(f);                           \
                                                }
// =========================[ FAKE FUNCTION DECLARATION ]==========================================
// ---------------------------------------------------------------------------------------
// Macros to craete fake function declarations
// ---------------------------------------------------------------------------------------
#define FK_DECLARE_FUNC_NAME_0(rt, f)           rt f()
#define FK_DECLARE_FUNC_NAME_1(rt, f, pt1)      rt f(pt1 a)
#define FK_DECLARE_FUNC_NAME_2(rt, f, pt1, pt2) rt f(pt1 a, pt2 b)
// ---------------------------------------------------------------------------------------
// Helper macros
// ---------------------------------------------------------------------------------------
#define FK_DECLARE_STRUCT_START(f)  typedef struct FAKE_STRUCT_TAG(f) { \
                                        int invoke_count
// -----
#define FK_DECLARE_STRUCT_END(f)  } FAKE_STRUCT_TAG(f);                 \
                                   extern FAKE_STRUCT_TAG(f) FAKE_STRUCT_VAR(f)
// ------
#define FK_STRUCT_OUT_FIELD(pt, p)  pt p
#define FK_STRUCT_IN_FIELD(pt, p) /* No need for a field for input param */

#define FK_STRUCT_FIELD(pd, pt, p) FK_STRUCT_## pd ## _FIELD(pt, p)
// ---------------------------------------------------------------------------------------
// Macros to define fake function structure for output and return parameters.
// ---------------------------------------------------------------------------------------
#define FK_DECLARE_STRUCT_VOID_0(f)                     FK_DECLARE_STRUCT_START(f); \
                                                        FK_DECLARE_STRUCT_END(f)
// ------
#define FK_DECLARE_STRUCT_VOID_1(f, pd1, pt1)           FK_DECLARE_STRUCT_START(f);       \
                                                            FK_STRUCT_FIELD(pd1, pt1, a); \
                                                        FK_DECLARE_STRUCT_END(f)
// ------
#define FK_DECLARE_STRUCT_VOID_2(f, pd1, pt1, pd2, pt2) FK_DECLARE_STRUCT_START(f);      \
                                                           FK_STRUCT_FIELD(pd1, pt1, a); \
                                                           FK_STRUCT_FIELD(pd2, pt2, b); \
                                                        FK_DECLARE_STRUCT_END(f)
// ---------------------------------------------------------------------------------------
#define FK_DECLARE_STRUCT_0(rt, f)                     FK_DECLARE_STRUCT_START(f);            \
                                                            FK_STRUCT_FIELD.g(OUT, rt, *ret); \
                                                       FK_DECLARE_STRUCT_END(f)
// ------
#define FK_DECLARE_STRUCT_1(rt, f, pd1, pt1)           FK_DECLARE_STRUCT_START(f);          \
                                                            FK_STRUCT_FIELD(OUT, rt, *ret); \
                                                            FK_STRUCT_FIELD(pd1, pt1, a);   \
                                                       FK_DECLARE_STRUCT_END(f)
// ------
#define FK_DECLARE_STRUCT_2(rt, f, pd1, pt1, pd2, pt2) FK_DECLARE_STRUCT_START(f);          \
                                                             FK_STRUCT_FIELD(OUT, rt, *ret);\
                                                             FK_STRUCT_FIELD(pd1, pt1, a);  \
                                                             FK_STRUCT_FIELD(pd2, pt2, b);  \
                                                         FK_DECLARE_STRUCT_END(f)
// ---------------------------------------------------------------------------------------
// Complete fake function and structure declarations
// ---------------------------------------------------------------------------------------
#define DECLARE_FUNC_VOID_0(f)           FK_DECLARE_STRUCT_VOID_0(f); \
                                         FK_DECLARE_FUNC_NAME_0(void, f)
// ------
#define DECLARE_FUNC_VOID_1(f, pd1, pt1) FK_DECLARE_STRUCT_VOID_1(f, pd1, pt1); \
                                         FK_DECLARE_FUNC_NAME_1(void, f, pt1)
// ------
#define DECLARE_FUNC_VOID_2(f, pd1, pt1, pd2, pt2) FK_DECLARE_STRUCT_VOID_2(f, pd1, pt1, pd2, pt2);\
                                                   FK_DECLARE_FUNC_NAME_2(void, f, pt1, pt2)
// ---------------------------------------------------------------------------------------
#define DECLARE_FUNC_0(rt, f)            FK_DECLARE_STRUCT_0(rt, f); \
                                         FK_DECLARE_FUNC_NAME_0(rt, f)
// ------
#define DECLARE_FUNC_1(rt, f, pd1, pt1) FK_DECLARE_STRUCT_1(rt, f, pd1, pt1); \
                                        FK_DECLARE_FUNC_NAME_1(rt, f, pt1)
// ------
#define DECLARE_FUNC_2(rt, f, pd1, pt1, pd2, pt2) FK_DECLARE_STRUCT_2(rt, f, pd1, pt1, pd2, pt2); \
                                                  FK_DECLARE_FUNC_NAME_2(rt, f, pt1, pt2)
// ---------------------------------------------------------------------------------------
#endif
