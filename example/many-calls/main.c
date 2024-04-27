#include <stdio.h>
#include <stdbool.h>
#include "../../fake.h"
#include "../../unittest.h"

/* Fake function defination and declaration */
DECLARE_FUNC (bool, isSpaceAvailable);
DEFINE_FUNC (bool, isSpaceAvailable);

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
TEST (Installation, Success_Complete)
{
    isSpaceAvailable_fake.ret = true;
    EQ_SCALAR (installation(), true);
    EQ_SCALAR (isSpaceAvailable_fake.invokeCount, LOOP_COUNT);
    END();
}

bool isSpaceAvailable_handler()
{
    int failAfter = *(int*)isSpaceAvailable_fake.resources; // Loop iterations.
    return (isSpaceAvailable_fake.invokeCount == failAfter) ? false : true;
}

TEST (Installation, Fail_Afterwards)
{
    int failAfter                   = 2; // Loop iterations.
    isSpaceAvailable_fake.resources = &failAfter;
    isSpaceAvailable_fake.handler   = isSpaceAvailable_handler;
    EQ_SCALAR (installation(), false);
    EQ_SCALAR (isSpaceAvailable_fake.invokeCount, failAfter);
    END();
}

void reset()
{
    RESET_FAKE (isSpaceAvailable);
}

int main()
{
    Success_Complete();
    Fail_Afterwards();
}
