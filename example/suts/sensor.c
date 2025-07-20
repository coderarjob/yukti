#include "sensor.h"
#include <stdint.h>

double read_temparature()
{
    // Note: Assume that log_adc_value, readADC functions is defined externally. For our case we
    // will use this to show how to mock external functions in yukti.h
    uint16_t adc = readADC();
    log_adc_value (adc);

    // Note: This calculation is just a sample to demonstrate yukti.h, it has no other purpose.

    // Assume that ADC value is 4mv/degree.
    double volts = ADC_RAIL_VOLTAGE * adc / ADC_MAX_VALUE;
    return volts / 4.0e-3;
}
