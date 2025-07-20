/* Unittest for suts/sensor.c */

#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"
#include "suts/sensor.h"

/*************************************************************************************
 * Mocking of readADC function
 ************************************************************************************/
YT_DECLARE_FUNC (uint16_t, readADC);
YT_DECLARE_FUNC_VOID (record_current_temparate, double);
YT_DECLARE_FUNC (uint32_t, pwm_adjust, double, double);
YT_DECLARE_FUNC_VOID (pwm_set, uint32_t);
YT_DECLARE_FUNC (bool, should_stop);

YT_DEFINE_FUNC (uint16_t, readADC);
YT_DEFINE_FUNC_VOID (record_current_temparate, double);
YT_DEFINE_FUNC (uint32_t, pwm_adjust, double, double);
YT_DEFINE_FUNC_VOID (pwm_set, uint32_t);
YT_DEFINE_FUNC (bool, should_stop);

/*************************************************************************************
 * Tests
 ************************************************************************************/
YT_TESTP (sensor, read_temparature_test, uint16_t, double)
{
    uint16_t adc_value     = YT_ARG_0();
    double temparature_exp = YT_ARG_1();

    // read_temparature() which calls readADC() should return the adc value we expect.
    readADC_fake.ret = adc_value;

    YT_EQ_DOUBLE (read_temparature(), temparature_exp, 0.00999);
    YT_END();
}

// Handler function is called when should_stop mocked function is called. What the handler function
// does has no effect as long as it returns what should_stop function expects.
bool should_stop_handler()
{
    // * `should_stop_fake.resources` is used to pass values from the test function. Since the
    // handler function is called from within the test function, we can pass stack variables through
    // this `resources`.
    int stop_after_iteration = *(int*)should_stop_fake.resources;

    // * `should_stop_fake.invokeCount` shows the number of times this mocked function was called.
    // Starts from 1.
    return (should_stop_fake.invokeCount > stop_after_iteration);
}

YT_TEST (sensor, control_temparature_test)
{
    // should_stop() function should return true after 1 iteration.
    int stop_after_iteration   = 1;
    should_stop_fake.resources = &stop_after_iteration;
    should_stop_fake.handler   = should_stop_handler;

    YT_MUST_CALL_IN_ORDER (readADC);
    YT_MUST_CALL_IN_ORDER (record_current_temparate, _); // _ means don't care argument.
    YT_MUST_CALL_IN_ORDER (pwm_adjust, _, YT_V (45.5)); // YT_V(..) macro is used to pass some value
                                                        // when its expected.
    YT_MUST_CALL_IN_ORDER (pwm_set, _);

    start_control_temparature (45.5);

    YT_END();
}

/*************************************************************************************
 * Reset Mock functions.
 * This function is automatically called before each test function is run.
 ************************************************************************************/
void yt_reset()
{
    YT_RESET_MOCK (readADC);
    YT_RESET_MOCK (record_current_temparate);
    YT_RESET_MOCK (pwm_adjust);
    YT_RESET_MOCK (pwm_set);
    YT_RESET_MOCK (should_stop);
}

int main()
{
    YT_INIT();
    // clang-format off
    read_temparature_test (3, YT_ARG (uint16_t){ 0, 24, 1023 },
                              YT_ARG (double){ 0.0, 29.32, 1250.0 });
    // clang_format on
    control_temparature_test();
    YT_RETURN_WITH_REPORT();
}
