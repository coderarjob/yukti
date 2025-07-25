#define YUKTI_TEST_STRIP_PREFIX
#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"

int func_extern_1 (int, int);
void func_extern_2 (int, int);

int sut_func (int a, int b)
{
    func_extern_2 (b, a);
    func_extern_2 (a, b);
    return func_extern_1 (a, b);
}

DECLARE_FUNC (int, func_extern_1, int, int);
DECLARE_FUNC_VOID (func_extern_2, int, int);
DECLARE_FUNC_VOID (func_extern_3);

DEFINE_FUNC (int, func_extern_1, int, int);
DEFINE_FUNC_VOID (func_extern_2, int, int);
DEFINE_FUNC_VOID (func_extern_3);

TEST (mock, must_never_call)
{
    MUST_NEVER_CALL (func_extern_3);
    sut_func (0, 0);

    END();
}

TEST (mock, must_never_call_with_arguments)
{
    MUST_NEVER_CALL (func_extern_2, V (0),
                        V (2)); // func_extern_2(1, 0), func_extern_2(0, 1) was called.
    MUST_NEVER_CALL (func_extern_1, _, V (0)); // func_extern_1(0, 1) was called.
    sut_func (0, 1);

    END();
}

TEST (mock, must_call_in_order)
{
    MUST_CALL_IN_ORDER (func_extern_2, V (1), V (0)); // func_extern_2(1, 0) was called.
    MUST_CALL_IN_ORDER (func_extern_1, _, V (1));        // func_extern_1(0, 1) was called.
    sut_func (0, 1);

    END();
}

TEST (mock, must_call_any_order)
{
    MUST_CALL_ANY_ORDER (func_extern_1, _, V (1));        // func_extern_1(0, 1) was called.
    MUST_CALL_ANY_ORDER (func_extern_2, V (1), V (0)); // func_extern_2(1, 0) was called.
    sut_func (0, 1);

    END();
}

TEST (mock, must_call_any_order_at_least_times)
{
    // func_extern_1(0, 1) was called.
    MUST_CALL_ANY_ORDER_ATLEAST_TIMES (1, func_extern_1, _, V (1));
    // func_extern_2(1, 0), func_extern_2(1, 0) was called.
    MUST_CALL_ANY_ORDER_ATLEAST_TIMES (2, func_extern_2, _, _);

    sut_func (0, 1);

    END();
}

TEST (mock, must_call_in_order_at_least_times)
{
    // func_extern_2(1, 0), func_extern_2(1, 0) was called.
    MUST_CALL_IN_ORDER_ATLEAST_TIMES (2, func_extern_2, _, _);
    // func_extern_1(0, 1) was called.
    MUST_CALL_IN_ORDER_ATLEAST_TIMES (1, func_extern_1, _, V (1));

    sut_func (0, 1);

    END();
}

void yt_reset()
{
    RESET_MOCK (func_extern_1);
    RESET_MOCK (func_extern_2);
    RESET_MOCK (func_extern_3);
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
    RETURN_WITH_REPORT();
}
