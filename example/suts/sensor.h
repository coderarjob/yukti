#ifndef SENSOR_H
#define SENSOR_H

#include <stdbool.h>
#include <stdint.h>

#define ADC_MAX_VALUE    (1023U)
#define ADC_RAIL_VOLTAGE (5.0)

double read_temparature();
void start_control_temparature (double goal_temparature);

// Note: Assume that that the following are defined externally. For our case we will
// use this to show how to mock external functions in yukti.h
uint16_t readADC();
void record_current_temparate (double);
uint32_t pwm_adjust (double, double);
void pwm_set (uint32_t);
bool should_stop();

#endif // SENSOR_H
