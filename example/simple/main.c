#include <stdio.h>
#include <stdbool.h>
#define YUKTI_TEST_DEBUG
#define YUKTI_TEST_IMPLEMENTATION
#include "../../yukti.h"

YT_DECLARE_FUNC (int, fnA);
YT_DEFINE_FUNC (int, fnA);

YT_DECLARE_FUNC_VOID (fnB, int, char, int);
YT_DEFINE_FUNC_VOID (fnB, int, char, int);

YT_DECLARE_FUNC_VOID (fnC);
YT_DEFINE_FUNC_VOID (fnC);

int fn_under_test (bool m)
{
    if (m) {
        // Branch 1
        fnB (11, 'b', 13);
        return fnA();
    } else {
        // Branch 2
        fnB (10, 'a', 13);
        fnA();

        // fnC();
        return 1;
    }
}

YT_TEST (simple, branch_1_test)
{
    fnA_fake.ret = 10;
    YT_EQ_SCALAR (fn_under_test (true), 10);

    // -------------------
    // TUTORIAL: The 1st & 3rd arguments are don't care. So changing them will not fail the test.
    // TUTORIAL: The 2nd argument is checked so its value must match the expectation in the test.
    // -------------------
    YT_MUST_CALL_IN_ORDER (fnB, _, YT_V ('b'), _);

    // -------------------
    // TUTORIAL: In the test we check the order of calls. fnB(..) must be called before fnA().
    // Change the order or comment out any of the calls and see the test fail.
    // -------------------
    YT_MUST_CALL_IN_ORDER (fnA);
    YT_END();
}

YT_TEST (simple, branch_2_test)
{
    YT_EQ_SCALAR (fn_under_test (false), 1);

    // -------------------
    // NOTE: It does not matter if we have a IN_ORDER or ANY_ORDER below, because with there must be
    // at least two IN_ORDER expectations to make sense of any order. A single IN_ORDER is same as
    // ANY_ORDER expectation.
    // -------------------
    YT_MUST_CALL_IN_ORDER (fnA);

    // -------------------
    // TUTORIAL: In this test fnB() can be called in any order. So changing the order will NOT
    // cause the test to fail.
    // -------------------
    // TUTORIAL: The 3rd argument is don't care. So changing that will cause the test to fail.
    // TUTORIAL: The 1st & 2nd arguments are checked so their values must match the expectation
    // in the test.
    // -------------------
    YT_MUST_CALL_ANY_ORDER (fnB, YT_V (10), YT_V ('a'), _);

    // -------------------
    // TUTORIAL: fnC() call is not expected. Uncomment the fnC() call and the MUST_NOT_CALL
    // expectation will fail.
    // -------------------
    YT_MUST_NEVER_CALL (fnC);
    YT_END();
}

void reset()
{
    YT_RESET_MOCK (fnA);
    YT_RESET_MOCK (fnB);
    YT_RESET_MOCK (fnC);
}

int main()
{
    YT_INIT();
    branch_1_test();
    branch_2_test();
    YT_RETURN_WITH_REPORT();
}
