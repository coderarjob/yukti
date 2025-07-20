#ifndef SENSOR_H
#define SENSOR_H

#include <stdbool.h>
#include <stdint.h>

#define ADC_MAX_VALUE    (1023U)
#define ADC_RAIL_VOLTAGE (5.0)

// Note: Assume that log_adc_value, readADC functions is defined externally. For our case we will
// use this to show how to mock external functions in yukti.h
uint16_t readADC();
void log_adc_value (uint16_t adc);

double read_temparature();

#endif // SENSOR_H
