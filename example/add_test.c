/* Unittest for suts/add.c 
 * Compile using `gcc example/add_test.c example/suts/add.c -o ./add_test`
 */

#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"
#include "suts/add.h"

/*************************************************************************************
 * Tests
 ************************************************************************************/
YT_TEST (add, add_zeros)
{
    YT_EQ_SCALAR (add (0, 0), 0);
    YT_END();
}

YT_TEST (add, add_two_negative_numbers)
{
    YT_EQ_SCALAR (add (-2, -1), -3);
    YT_END();
}

YT_TEST (add, add_two_positive_numbers)
{
    YT_EQ_SCALAR (add (2, 1), 3);
    YT_END();
}

YT_TEST (add, add_must_fail)
{
    YT_NEQ_SCALAR (add (65536, 1), 65537);
    YT_END();
}

YT_TEST (add, add_cascade)
{
    YT_EQ_SCALAR (add (add (1, 2), 3), 6);
    YT_END();
}

/*************************************************************************************
 * Reset Mock functions.
 * This function is automatically called before each test function is run.
 ************************************************************************************/
void yt_reset()
{
}

int main (void)
{
    YT_INIT();
    add_zeros();
    add_two_negative_numbers();
    add_two_positive_numbers();
    add_must_fail();
    add_cascade();
    YT_RETURN_WITH_REPORT();
}
