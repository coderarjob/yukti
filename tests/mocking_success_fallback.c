#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"

typedef struct {
    int a;
} SomeType;

int func_extern_1 (SomeType, int);
int sut_func (SomeType a, int b)
{
    return func_extern_1 (a, b);
}

YT_DECLARE_FUNC (int, func_extern_1, SomeType, int);
YT_DEFINE_FUNC_FALLBACK (int, func_extern_1, SomeType, int);

SomeType dummy = { 0 };

YT_TEST (mock, mock_return_value)
{
    func_extern_1_fake.ret = 0xFABC;

    YT_EQ_SCALAR (sut_func (dummy, 0), 0xFABC);

    YT_END();
}

int func_extern_1_handler_1 (SomeType a, int b)
{
    YT_EQ_MEM (&a, &dummy, sizeof (a));
    YT_EQ_SCALAR (b, 0x2222);
    return 0;
}

YT_TEST (mock, mock_handler)
{
    func_extern_1_fake.handler = func_extern_1_handler_1;
    YT_EQ_SCALAR (sut_func (dummy, 0x2222), 0x0);

    YT_END();
}

int func_extern_1_handler_2 (SomeType a, int b)
{
    (void)a;
    (void)b;
    YT_EQ_SCALAR (*(int*)func_extern_1_fake.resources, 0x1212);
    return 0;
}

YT_TEST (mock, mock_handler_resource_passing)
{
    int resource                 = 0x1212;
    func_extern_1_fake.resources = &resource;
    func_extern_1_fake.handler   = func_extern_1_handler_2;
    YT_EQ_SCALAR (sut_func (dummy, 0), 0);

    YT_END();
}

void yt_reset()
{
    YT_RESET_MOCK (func_extern_1);
}

int main (void)
{
    YT_INIT();
    mock_return_value();
    mock_handler();
    mock_handler_resource_passing();
    YT_RETURN_WITH_REPORT();
}
