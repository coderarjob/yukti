/* Unittest for suts/sensor.c */

#include <stdint.h>
#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"
#include "suts/sensor.h"

/*************************************************************************************
 * Mocking of readADC function
 ************************************************************************************/
YT_DECLARE_FUNC (uint16_t, readADC);
YT_DECLARE_FUNC_VOID (log_adc_value, uint16_t);
YT_DEFINE_FUNC (uint16_t, readADC);
YT_DEFINE_FUNC_VOID (log_adc_value, uint16_t);

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

YT_TEST (sensor, read_temparature_log_called)
{
    readADC_fake.ret = 10; // Just a dummy value that readADC() should return.

    // Expect: log_adc_value(10) called.
    YT_MUST_CALL_IN_ORDER (log_adc_value, YT_V (10));

    read_temparature();
    YT_END();
}

/*************************************************************************************
 * Reset Mock functions.
 * This function is automatically called before each test function is run.
 ************************************************************************************/
void yt_reset()
{
    YT_RESET_MOCK (readADC);
    YT_RESET_MOCK (log_adc_value);
}

int main()
{
    YT_INIT();
    // clang-format off
    read_temparature_test (3, YT_ARG (uint16_t){ 0, 24, 1023 },
                              YT_ARG (double){ 0.0, 29.32, 1250.0 });
    // clang_format on
    read_temparature_log_called();
    YT_RETURN_WITH_REPORT();
}
