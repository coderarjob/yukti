#include <stdio.h>
#include <stdbool.h>
#define YUKTI_TEST_IMPLEMENTATION
#include "../../yukti.h"

YT_DECLARE_FUNC (int, fnA);
YT_DEFINE_FUNC (int, fnA);

YT_DECLARE_FUNC_VOID (fnB, int, char, int);
YT_DEFINE_FUNC_VOID (fnB, int, char, int);

int fn_under_test (bool m)
{
    if (m) {
        // Branch 1
        return fnA();
    } else {
        // Branch 2
        fnB(10,'a',13);
        return 1;
    }
}

YT_TEST(simple, branch_1_test)
{
    fnA_fake.ret = 10;
    YT_EQ_SCALAR(fn_under_test(true), 10);
    YT_MUST_CALL_ANY_ORDER(fnA);
    YT_END();
}

YT_TEST(simple, branch_2_test)
{
    YT_EQ_SCALAR(fn_under_test(false), 1);
    YT_MUST_CALL_ANY_ORDER(fnB,YT_V(10),YT_V('a'),_);
    YT_END();
}

void reset()
{
    YT_RESET_MOCK(fnA);
    YT_RESET_MOCK(fnB);
}

int main()
{
    branch_1_test();
    branch_2_test();
    return 0;
}
