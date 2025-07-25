#define YUKTI_TEST_STRIP_PREFIX
#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"

int add(int a, int b)
{
    return a + b;
}

TESTP (add, add_numbers, int, int, int)
{
    int n1  = ARG_0();
    int n2  = ARG_1();
    int exp = ARG_2();

    EQ_SCALAR (add (n1, n2), exp);
    END();
}


void yt_reset()
{
}

int main (void)
{
    YT_INIT();

    // clang-format off
    add_numbers (5, ARG (int){ 0, -1, 1, -1, 4 }, // 1st arguments to test function
                    ARG (int){ 0, -2, 2, 4, -1 }, // 2nd arguments to test function
                    ARG (int){ 0, -3, 3, 3, 3 }); // 3rd arguments to test function
    // clang-format off

    RETURN_WITH_REPORT();
}
