#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"

int func_extern_3();
void func_extern_2 (int, int);

int sut_func (int a, int b)
{
    func_extern_2 (b, a);
    func_extern_2 (a, b);
    return func_extern_3();
}

YT_DECLARE_FUNC (int, func_extern_1, int, int);
YT_DECLARE_FUNC_VOID (func_extern_2, int, int);
YT_DECLARE_FUNC (int, func_extern_3);

YT_DEFINE_FUNC (int, func_extern_1, int, int);
YT_DEFINE_FUNC_VOID (func_extern_2, int, int);
YT_DEFINE_FUNC (int, func_extern_3);

YT_TEST (mock, must_never_call)
{
    YT_MUST_NEVER_CALL (func_extern_3);
    sut_func (0, 0);

    YT_END();
}

YT_TEST (mock, must_never_call_with_arguments)
{
    // func_extern_2(1, 0), func_extern_2(0, 1) was called.
    YT_MUST_NEVER_CALL (func_extern_2, YT_V (0), YT_V (1));
    YT_MUST_NEVER_CALL (func_extern_2, YT_V (1), YT_V (0));
    sut_func (0, 1);

    YT_END();
}

YT_TEST (mock, must_call_in_order)
{
    YT_MUST_CALL_IN_ORDER (func_extern_3);
    YT_MUST_CALL_IN_ORDER (func_extern_2, YT_V (1), YT_V (0));
    sut_func (0, 1);

    YT_END();
}

YT_TEST (mock, must_call_any_order)
{
    // func_extern_2(1, 0), func_extern_2(1, 0) was called.
    YT_MUST_CALL_ANY_ORDER (func_extern_2, _, YT_V (2));
    YT_MUST_CALL_ANY_ORDER (func_extern_2, YT_V (2), YT_V (0));
    sut_func (0, 1);

    YT_END();
}

YT_TEST (mock, must_call_any_order_at_least_times)
{
    // func_extern_2(1, 0), func_extern_2(1, 0) was called.
    YT_MUST_CALL_ANY_ORDER_ATLEAST_TIMES (3, func_extern_2, _, _);

    sut_func (0, 1);

    YT_END();
}

YT_TEST (mock, must_call_in_order_at_least_times)
{
    // func_extern_2(1, 0), func_extern_2(1, 0) was called.
    YT_MUST_CALL_IN_ORDER_ATLEAST_TIMES (3, func_extern_2, _, _);
    // func_extern_1(0, 1) was called.
    YT_MUST_CALL_IN_ORDER_ATLEAST_TIMES (2, func_extern_3);

    sut_func (0, 1);

    YT_END();
}

void yt_reset()
{
    YT_RESET_MOCK (func_extern_1);
    YT_RESET_MOCK (func_extern_2);
    YT_RESET_MOCK (func_extern_3);
}

int main (void)
{
    YT_INIT();
    must_never_call();
    must_never_call_with_arguments();
    must_call_in_order();
    must_call_any_order();
    must_call_any_order_at_least_times();
    must_call_in_order_at_least_times();
    YT_RETURN_WITH_REPORT();
}
