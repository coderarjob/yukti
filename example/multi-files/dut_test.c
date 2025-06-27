#define YUKTI_TEST_IMPLEMENTATION
#include "../../yukti.h"
#include "mock.h"
#include <stdint.h>

void funcA();
void funcB(uint32_t);

void dut()
{
    funcA();
    funcB(10);
}

YT_TEST(test, dut_test)
{
    YT_MUST_CALL_IN_ORDER(funcA);
    YT_MUST_CALL_IN_ORDER(funcB, YT_V(10));
    dut();
    YT_END();
}

void reset()
{
    reset_mock();
}

int main(void)
{
    YT_INIT();
    dut_test();
    YT_RETURN_WITH_REPORT();
}
