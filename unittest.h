/*
 * ----------------------------------------------------------------------------
 * Macros for equality and negation checks.
 *
 * Author: arjobmukherjee@gmail.com (Arjob Mukherjee)
 * Dated : 26 April 2022
 * ----------------------------------------------------------------------------
 */
#ifndef UNITTEST_H
#define UNITTEST_H
#include <stddef.h>
#include <stdio.h>
#include "fake.h"

int ut_equal_mem (void *a, void *b, size_t size, int *i);
int ut_equal_string (char *a, char *b, int *i);

#define COL_RED   "\x1b[31m"
#define COL_GREEN "\x1b[32m"
#define COL_RESET "\x1b[0m"

#define UT_REPORT_PASS(t) printf ("\n  %-20s : %s PASS %s",                   \
                                     #t, COL_GREEN, COL_RESET)

#define UT_REPORT_FAIL(t) printf ("\n  %-20s : %s FAIL %s",                   \
                                     #t, COL_RED, COL_RESET)

#define TEST(tf, fn) static void fn () {                                      \
                        reset();                                              \
                        printf ("\n[TEST (%s)] %s", #tf, #fn); do
#define END()       } while(0);

#define EQ_SCALAR(a,b)  do {                                                  \
                            if (a==b)                                         \
                            { UT_REPORT_PASS(a == b); }                       \
                            else                                              \
                            { UT_REPORT_FAIL(a == b);                         \
                              printf("[Got: %d, Exp: %d]", a, b);             \
                            }} while(0)

#define EQ_MEM(a,b,sz)  do {                                                  \
                            int i;                                            \
                            if (ut_equal_mem (a,b, sz, &i))                   \
                            { UT_REPORT_PASS(a == b); }                       \
                            else                                              \
                            { UT_REPORT_FAIL(a == b);                         \
                              printf("[Index: %d, Got: 0x%X, Exp: 0x%X]",     \
                                      i,a[i],b[i]);                           \
                            }} while(0)

#define EQ_STRING(a,b)  do {                                                  \
                            int i;                                            \
                            if (ut_equal_string (a, b, &i))                   \
                            { UT_REPORT_PASS(a == b); }                       \
                            else                                              \
                            { UT_REPORT_FAIL(a == b);                         \
                              printf("[Index: %d, Got: %c, Exp: %c]",         \
                                      i,a[i],b[i]);                           \
                            }} while(0)
#endif // UNITTEST_H
