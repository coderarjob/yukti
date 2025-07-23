/* Unittest for suts/sensor.c
 * Compile using `gcc example/sensor_test.c example/suts/sensor.c -o ./sensor_test`
 */

#include <stdbool.h>
#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"
#include "suts/sensor.h"

/*************************************************************************************
 * Mocking of readADC function
 ************************************************************************************/
YT_DECLARE_FUNC (uint16_t, readADC);
YT_DECLARE_FUNC (int, start_printing, const char*);
YT_DECLARE_FUNC (bool, is_printing_complete, int);
YT_DECLARE_FUNC_VOID (printer_report_progress, int);
YT_DECLARE_FUNC_VOID (set_status, Status);

YT_DEFINE_FUNC (uint16_t, readADC);
YT_DEFINE_FUNC (int, start_printing, const char*);
YT_DEFINE_FUNC (bool, is_printing_complete, int);
YT_DEFINE_FUNC_VOID (printer_report_progress, int);
YT_DEFINE_FUNC_VOID (set_status, Status);

/*************************************************************************************
 * Tests
 ************************************************************************************/
YT_TESTP (sensor, read_temperature_test, uint16_t, double)
{
    uint16_t adc_value     = YT_ARG_0();
    double temperature_exp = YT_ARG_1();

    // readADC() called by read_temperature() SUT function should return the adc value we expect.
    readADC_fake.ret = adc_value;

    YT_EQ_DOUBLE (read_temperature(), temperature_exp, 0.00999);
    YT_END();
}

YT_TEST (printer, printer_fail)
{
#define DUMMY_FILE_NAME "./somefile"

    // In order to simulate printer error, we want start_printing() function to return a negative
    // number. start_printing() is called by the print_and_wait() SUT function.
    start_printing_fake.ret = -1;

    // Since printing has failed, we expect `set_status (STATUS_ERROR)` to be called. ANY_ORDER
    // macro is used because we don't care in what order this is called.
    YT_MUST_CALL_ANY_ORDER (set_status, YT_V (STATUS_ERROR));

    // Call the actual SUT function.
    print_and_wait (DUMMY_FILE_NAME);
    YT_END();
}

// Handler function is called when `is_printing_complete` mocked function is called. It must have
// the same signature as its mocked function, i.e `is_printing_complete` function here. What the
// handler function does has no effect as long as it returns what `is_printing_complete` function
// expects.
bool is_printing_complete_handler (int id)
{
    (void)id; // unused argument.

    // * `is_printing_complete.resources` is used to pass values from the test function. Since the
    // handler function is called from within the test function, we can pass stack variables through
    // this `resources`.
    int stop_after_iteration = *(int*)is_printing_complete_fake.resources;

    // * `is_printing_complete.invokeCount` shows the number of times this mocked function was
    // called. Starting from 1.
    return (is_printing_complete_fake.invokeCount > stop_after_iteration);
}

YT_TEST (printer, printer_success)
{
#define DUMMY_FILE_NAME "./somefile"

    // In order to simulate printer error, we want start_printing() function to return a
    // non-negative number. This step is OPTIONAL, since all fakes by default returns 0.
    start_printing_fake.ret = 0;

    // is_printing_complete() function should return false for 2 iterations, then it should return
    // true to break out of the `while` loop in the print_and_wait() SUT function. The handler
    // function contains this logic. To make it more flexible, we can pass in the number of
    // iterations in is_printing_complete.resources.
    int stop_after_iteration            = 2;
    is_printing_complete_fake.resources = &stop_after_iteration;
    is_printing_complete_fake.handler   = is_printing_complete_handler;

    // Since this is the success case we do not expect `set_status (STATUS_ERROR)` to ever be
    // called.
    YT_MUST_NEVER_CALL (set_status, YT_V (STATUS_ERROR));

    // We expect these functions to be called in the following order - start_printing() first
    // followed by is_printing_complete() and so on. Note that print_and_wait() function might be
    // calling more functions, but we only validate expectations on functions which we care about in
    // the test.

    // We can put expectations on what arguments were passed. There are two macros of this:
    // * 'YT_V(..)' macro is used to pass some value when its expected. Here we expect
    // `start_printing` to be called with the file name we passed to the SUT function
    YT_MUST_CALL_IN_ORDER (start_printing, YT_V (DUMMY_FILE_NAME));

    // * '_' means don't care argument. That is we just expect 'is_printing_complete' to be
    // called, but do not care about what argument was passed to it.
    YT_MUST_CALL_ANY_ORDER_ATLEAST_TIMES (stop_after_iteration, is_printing_complete, _);
    YT_MUST_CALL_ANY_ORDER_ATLEAST_TIMES (stop_after_iteration, printer_report_progress, _);

    YT_MUST_CALL_IN_ORDER (set_status, YT_V (STATUS_FINISHED));

    // Call the actual SUT function.
    print_and_wait (DUMMY_FILE_NAME);
    YT_END();
}

/*************************************************************************************
 * Reset Mock functions.
 * This function is automatically called before each test function is run.
 ************************************************************************************/
void yt_reset()
{
    YT_RESET_MOCK (readADC);
    YT_RESET_MOCK (start_printing);
    YT_RESET_MOCK (is_printing_complete);
    YT_RESET_MOCK (printer_report_progress);
    YT_RESET_MOCK (set_status);
}

int main()
{
    YT_INIT();
    // clang-format off
    read_temperature_test (3, YT_ARG (uint16_t){ 0, 24, 1023 },
                              YT_ARG (double){ 0.0, 29.32, 1250.0 });
    // clang_format on
    printer_fail();
    printer_success();
    YT_RETURN_WITH_REPORT();
}
