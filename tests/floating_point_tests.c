#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"
#include <stdint.h>
#include <float.h>

YT_TEST (fp, zero_equals)
{
    double a = 67329.234; // Just some number
    double b = 67329.234;

    YT_EQ_DOUBLE_ABS (a, b, FLT_EPSILON);
    YT_EQ_DOUBLE_ABS (0.0, a - b, FLT_EPSILON);
    YT_EQ_DOUBLE_ABS (0.0, b - a, FLT_EPSILON);
    YT_EQ_DOUBLE_ABS (-0.0, a - b, FLT_EPSILON);
    YT_EQ_DOUBLE_ABS (-0.0, b - a, FLT_EPSILON);

    YT_EQ_DOUBLE_REL (a, b, FLT_EPSILON);
    YT_EQ_DOUBLE_REL (0.0, a - b, FLT_EPSILON);
    YT_EQ_DOUBLE_REL (0.0, b - a, FLT_EPSILON);
    YT_EQ_DOUBLE_REL (-0.0, a - b, FLT_EPSILON);
    YT_EQ_DOUBLE_REL (-0.0, b - a, FLT_EPSILON);
    YT_END();
}

YT_TEST (fp, infinities)
{
    double a = -INFINITY;
    double b = INFINITY;
    double c = 0.1;

    YT_EQ_DOUBLE_ABS (a, b, FLT_EPSILON);
    YT_NEQ_DOUBLE_ABS (a, c, FLT_EPSILON);

    YT_EQ_DOUBLE_REL (a, b, FLT_EPSILON);
    YT_NEQ_DOUBLE_REL (a, c, FLT_EPSILON);
    YT_END();
}

YT_TEST (fp, nans)
{
    double a = -NAN;
    double b = NAN;
    double c = INFINITY;
    double d = 0.1;

    YT_NEQ_DOUBLE_ABS (a, b, FLT_EPSILON);
    YT_NEQ_DOUBLE_ABS (a, c, FLT_EPSILON);
    YT_NEQ_DOUBLE_ABS (a, d, FLT_EPSILON);

    YT_NEQ_DOUBLE_REL (a, b, FLT_EPSILON);
    YT_NEQ_DOUBLE_REL (a, c, FLT_EPSILON);
    YT_NEQ_DOUBLE_REL (a, d, FLT_EPSILON);
    YT_END();
}

YT_TESTP (fp, comparisons, double)
{
    double start = YT_ARG_0();

    double product = 0;
    for (int i = 0; i < 100; i++) {
        product += start;
    }

    YT_EQ_DOUBLE_REL (product, start * 100.0, FLT_EPSILON);
    YT_END();
}

void yt_reset()
{
}

int main (void)
{
    YT_INIT();
    zero_equals();
    infinities();
    nans();
    comparisons (6, YT_ARG (double){ 0.16777226, 0.1, -0.1, 5.0, -16777226.1, 16777226.1 });
    YT_RETURN_WITH_REPORT();
}
