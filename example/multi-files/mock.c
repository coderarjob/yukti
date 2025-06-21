#include <stdint.h>
#include "../../yukti.h"
#include "mock.h"

YT_DEFINE_FUNC_VOID(funcA);
YT_DEFINE_FUNC_VOID(funcB, uint32_t);

void reset_mock()
{
    YT_RESET_MOCK(funcA);
    YT_RESET_MOCK(funcB);
}

