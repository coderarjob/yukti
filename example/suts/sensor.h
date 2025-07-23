#ifndef SENSOR_H
#define SENSOR_H

#include <stdbool.h>
#include <stdint.h>

#define ADC_MAX_VALUE    (1023U)
#define ADC_RAIL_VOLTAGE (5.0)

double read_temperature();
void print_and_wait (const char* file);

typedef enum Status {
    STATUS_ERROR,
    STATUS_STARTED,
    STATUS_FINISHED
} Status;

// Note: Assume that that the following are defined externally. For our case we will
// use this to show how to mock external functions in yukti.h
uint16_t readADC();
int start_printing (const char*);
bool is_printing_complete (int);
void printer_report_progress (int);
void set_status (Status);

#endif // SENSOR_H
