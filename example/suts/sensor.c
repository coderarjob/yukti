#include "sensor.h"
#include <stdbool.h>
#include <stdint.h>

double read_temperature()
{
    // Note: Assume that readADC functions is defined externally. For our case we will use this to
    // show how to mock external functions in yukti.h
    uint16_t adc = readADC();

    // Note: This calculation is just a sample to demonstrate yukti.h, it has no other purpose.

    // Assume that ADC value is 4mv/degree.
    double volts = ADC_RAIL_VOLTAGE * adc / ADC_MAX_VALUE;
    return volts / 4.0e-3;
}

void print_and_wait (const char* file)
{
    // Start the printing
    int id = start_printing (file);
    if (id < 0) {
        set_status (STATUS_ERROR);
        return;
    }

    // Wait for printing to finish
    while (!is_printing_complete (id)) {
        printer_report_progress (id);
    }

    set_status (STATUS_FINISHED);
}
