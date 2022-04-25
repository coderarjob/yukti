/*
 * ------------------------------------------------------------------------------------------------
 * Macros for equality and negation checks.
 *
 * Author: arjobmukherjee@gmail.com (Arjob Mukherjee)
 * Dated : 26 April 2022
 * ------------------------------------------------------------------------------------------------
 */
#ifndef UNITTEST_H
#define UNITTEST_H

#define CHECK(t) (t) ? printf ("%s : PASS\n", __func__) \
                     : printf ("%s : FAIL (%s)\n", __func__, #t)
#define EQ(a,b)  CHECK(a==b)
#define NEQ(a,b) CHECK(a!=b)

#endif // UNITTEST_H
