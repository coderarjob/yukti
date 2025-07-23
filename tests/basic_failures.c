#define YUKTI_TEST_IMPLEMENTATION
#include "../yukti.h"

YT_TEST (basic, scaler_tests_floats)
{
    YT_NEQ_DOUBLE (1.0, 1.0, 0.01);     // Fails because 1.0 == 1.0
    YT_NEQ_DOUBLE (1.1234, 1.12, 0.01); // Fails because 1.1234 - 1.12 <= 0.01. Meaning the two are
                                        // treated as equal.

    YT_EQ_DOUBLE (1.1234, 1.12, 0.001); // Fails because 1.1234 - 1.12 > 0.001
    YT_EQ_DOUBLE (0.0, 1.0, 0.001);     // Fails because 0.0 != 1.0

    YT_END();
}

YT_TEST (basic, scaler_tests_ints)
{
    YT_NEQ_SCALAR (1, 1); // Fails because 1 == 1
    YT_EQ_SCALAR (3, 4);  // Fails because 3 != 4

    YT_GEQ_SCALAR (2, 3); // Fails because 2 < 3

    YT_LEQ_SCALAR (2, -2); // Fails because 2 > -2

    YT_LES_SCALAR (0, -1); // Fails because 0 > -1
    YT_GRT_SCALAR (1, 3);  // Fails because 1 < 3

    YT_END();
}

 YT_TEST (basic, string_tests)
{
    char* string1 = "Earth";

    YT_EQ_STRING (string1, "earth");  // Fails because string1 != "earth"
    YT_NEQ_STRING (string1, "Earth");  // Fails because string1 == "Earth"

    YT_END();
}

 YT_TEST (basic, memory_tests)
{
#define ARRAY_LEN(b) (sizeof (b) / sizeof (b[0]))

    char buffer1[] = { 0, 1, 2, 3 };
    //char buffer2[] = { 0, 1, 2, 3 };
    char buffer3[] = { 0, 1, 2, 4 };

    YT_EQ_MEM (buffer1, buffer3, ARRAY_LEN (buffer1));  // Fails because buffer1 != buffer3.
    //YT_NEQ_MEM (buffer1, buffer2, ARRAY_LEN (buffer1)); // Fails because buffer1 == buffer2.

    YT_END();
}

void yt_reset()
{
}

int main (void)
{
    YT_INIT();
    scaler_tests_ints();
    scaler_tests_floats();
    string_tests();
    memory_tests();
    YT_RETURN_WITH_REPORT();
}
