#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"

int add(int a, int b)
{
    return a + b;
}

YT_TESTP (add, add_numbers, int, int, int)
{
    int n1  = YT_ARG_0();
    int n2  = YT_ARG_1();
    int exp = YT_ARG_2();

    YT_EQ_SCALAR (add (n1, n2), exp);
    YT_END();
}


void yt_reset()
{
}

int main (void)
{
    YT_INIT();

    // clang-format off
    add_numbers (5, YT_ARG (int){ 0, -1, 1, -1, 4 }, // 1st arguments to test function
                    YT_ARG (int){ 0, -2, 2, 4, -1 }, // 2nd arguments to test function
                    YT_ARG (int){ 0, -3, 3, 3, 3 }); // 3rd arguments to test function
    // clang-format off

    YT_RETURN_WITH_REPORT();
}
