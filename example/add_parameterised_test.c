/* Unittest for suts/add.c */

#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"
#include "suts/add.h"

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

void yt_reset()
{
}

int main (void)
{
    YT_INIT();
    add_numbers (4, YT_ARG (int16_t){ 0, -1, 1, -1, 4 }, YT_ARG (int16_t){ 0, -2, 2, 4, -1 },
                 YT_ARG (int32_t){ 0, -3, 3, 3, 3 });
    add_must_fail();
    add_cascade();
    YT_RETURN_WITH_REPORT();
}
