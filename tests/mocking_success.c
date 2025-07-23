#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"

int func_extern_1 (int, int);

int sut_func (int a, int b)
{
    return func_extern_1 (a, b);
}

YT_DECLARE_FUNC (int, func_extern_1, int, int);
YT_DEFINE_FUNC (int, func_extern_1, int, int);

YT_TEST (mock, mock_return_value)
{
    func_extern_1_fake.ret = 0xFABC;

    YT_EQ_SCALAR (sut_func (0, 0), 0xFABC);

    YT_END();
}

int func_extern_1_handler_1 (int a, int b)
{
    YT_EQ_SCALAR (a, 0x1111);
    YT_EQ_SCALAR (b, 0x2222);
    return a + b;
}

YT_TEST (mock, mock_handler)
{
    func_extern_1_fake.handler = func_extern_1_handler_1;
    YT_EQ_SCALAR (sut_func (0x1111, 0x2222), 0x3333);

    YT_END();
}

int func_extern_1_handler_2 (int a, int b)
{
    YT_EQ_SCALAR (*(int*)func_extern_1_fake.resources, 0x1212);
    return a + b;
}

YT_TEST (mock, mock_handler_resource_passing)
{
    int resource                 = 0x1212;
    func_extern_1_fake.resources = &resource;
    func_extern_1_fake.handler   = func_extern_1_handler_2;
    YT_EQ_SCALAR (sut_func (0, 0), 0);

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
