#include <stdio.h>
#include <stdbool.h>
#define YUKTI_TEST_IMPLEMENTATION
#include "../../yukti.h"

/* Fake function defination and declaration */
YT_DECLARE_FUNC (bool, isSpaceAvailable);
YT_DEFINE_FUNC (bool, isSpaceAvailable);

/* Function under test */
#define LOOP_COUNT 10

bool installation()
{
    for (int i = 1; i <= LOOP_COUNT; i++) {
        printf ("\nInstalling file #%d", i);
        if (!isSpaceAvailable())
            return false;
    }
    return true;
}

/* Tests to test the `function under test` */
YT_TEST (Installation, Success_Complete)
{
    isSpaceAvailable_fake.ret = true;
    YT_EQ_SCALAR (installation(), true);
    YT_EQ_SCALAR (isSpaceAvailable_fake.invokeCount, (unsigned)LOOP_COUNT);
    YT_END();
}

bool isSpaceAvailable_handler()
{
    int failAfter = *(int*)isSpaceAvailable_fake.resources; // Loop iterations.
    return (isSpaceAvailable_fake.invokeCount == (unsigned)failAfter) ? false : true;
}

YT_TEST (Installation, Fail_Afterwards)
{
    int failAfter                   = 2; // Loop iterations.
    isSpaceAvailable_fake.resources = &failAfter;
    isSpaceAvailable_fake.handler   = isSpaceAvailable_handler;
    YT_EQ_SCALAR (installation(), false);
    YT_EQ_SCALAR (isSpaceAvailable_fake.invokeCount, (unsigned)failAfter);
    YT_END();
}

void reset()
{
    YT_RESET_MOCK (isSpaceAvailable);
}

int main()
{
    Success_Complete();
    Fail_Afterwards();
    YT_RETURN_WITH_REPORT();
}
