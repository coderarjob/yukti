/*
 * ----------------------------------------------------------------------------
 * Functions for equality and negation checks.
 *
 * Author: arjobmukherjee@gmail.com (Arjob Mukherjee)
 * Dated : 26 April 2022
 * ----------------------------------------------------------------------------
 */
#include "unittest.h"

int ut_equal_string (char *a, char *b, int *i)
{
    *i = 0;
    while (*a && *b && *a == *b) {
        a++; b++; (*i)++;
    }

    return *a == *b;
}

int ut_equal_mem (void *a, void *b, size_t size, int *i)
{
    *i = 0;
    while (size-- && *(char *)a++ == *(char *)b++)
        (*i)++ ;

    return *(char *)--a == *(char *)--b;
}
