#define YUKTI_TEST_IMPLEMENTATION
#define YUKTI_TEST_DEBUG
#include "../yukti.h"

int func_extern_1 (int, int);
void func_extern_2 (int, int);

int sut_func (int a, int b)
{
    func_extern_2 (b, a);
    return func_extern_1 (a, b);
}

YT_DECLARE_FUNC (int, func_extern_1, int, int);
YT_DECLARE_FUNC_VOID (func_extern_2, int, int);

YT_DEFINE_FUNC (int, func_extern_1, int, int);
YT_DEFINE_FUNC_VOID (func_extern_2, int, int);

YT_TEST (mock, must_call_in_order)
{
    YT_MUST_CALL_IN_ORDER(func_extern_1);
    YT_MUST_CALL_IN_ORDER(func_extern_2, YT_V(1), _);
    sut_func (0, 0);

    YT_END();
}

void yt_reset()
{
    YT_RESET_MOCK (func_extern_1);
    YT_RESET_MOCK (func_extern_2);
}

int main (void)
{
    YT_INIT();
    must_call_in_order();
    YT_RETURN_WITH_REPORT();
}
