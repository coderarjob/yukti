/* Unittest for suts/add.c
 * Compile using `gcc example/add_parameterised_test.c example/suts/add.c -o ./add_param_test`
 */

#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"
#include "suts/add.h"

/*************************************************************************************
 * Tests
 ************************************************************************************/
YT_TESTP (add, add_numbers, int16_t, int16_t, int32_t)
{
    int16_t n1  = YT_ARG_0();
    int16_t n2  = YT_ARG_1();
    int16_t exp = YT_ARG_2();

    YT_EQ_SCALAR (add (n1, n2), exp);
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

    // There are 5 test cases, i.e 5 set of values are passed to add() SUT function.
    // 1st case: YT_EQ_SCALAR (add (0, 0), 0);
    // 2nd case: YT_EQ_SCALAR (add (-1, -2), -3);
    // 3rd case: YT_EQ_SCALAR (add (1, 2), 3);
    // 4th case: YT_EQ_SCALAR (add (-1, 4), 3);
    // 5th case: YT_EQ_SCALAR (add (4, -1), 3);

    // clang-format off
    add_numbers (5, YT_ARG (int16_t){ 0, -1, 1, -1, 4 }, // 1st arguments to test function
                    YT_ARG (int16_t){ 0, -2, 2, 4, -1 }, // 2nd arguments to test function
                    YT_ARG (int32_t){ 0, -3, 3, 3, 3 }); // 3rd arguments to test function
    // clang-format off

    add_must_fail();
    add_cascade();
    YT_RETURN_WITH_REPORT();
}
