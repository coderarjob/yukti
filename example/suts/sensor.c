#include "sensor.h"
#include <stdbool.h>
#include <stdint.h>

double read_temparature()
{
    // Note: Assume that readADC functions is defined externally. For our case we will use this to
    // show how to mock external functions in yukti.h
    uint16_t adc = readADC();

    // Note: This calculation is just a sample to demonstrate yukti.h, it has no other purpose.

    // Assume that ADC value is 4mv/degree.
    double volts = ADC_RAIL_VOLTAGE * adc / ADC_MAX_VALUE;
    return volts / 4.0e-3;
}

void start_control_temparature (double goal_temparature)
{
    while (!should_stop()) {
        double t = read_temparature();
        record_current_temparate (t);

        uint32_t pwm = pwm_adjust (t, goal_temparature);
        pwm_set (pwm);
    }
}
