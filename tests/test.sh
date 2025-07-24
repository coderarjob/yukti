#!/bin/bash

TMP_DIR=/tmp

#######################################################################
# Running recorded tests
#######################################################################
echo "Test 1/2: Running recorded tests"

TESTS_DIR=`dirname $0`
EXP_DIR="$TESTS_DIR/exps"

if [ ! -e $EXP_DIR ]; then
    echo " Fatal: Expectations folder not found. Exiting!"
    exit 1
fi

for fn in $TESTS_DIR/*.c; do
    fname_we=${fn##*/}              # File title with extension
    fname=${fname_we%%.c}           # File title without extension

    bin_file=$TESTS_DIR/test        # Compiled test binary name
    out_file=$TESTS_DIR/test.out    # Output from running the test
    exp_file=$EXP_DIR/$fname.exp    # Expectation file for test

    echo " Compiling and running '$fn'"
    gcc -Wall -Wextra $fn -o $bin_file || exit

    $bin_file > $out_file
    diff $out_file $exp_file || exit
done

#######################################################################
# Compiling and running examples
#######################################################################
echo "Test 2/2: Running examples"

echo " Running 'Sensor' example.."
gcc example/sensor_test.c \
    example/suts/sensor.c \
    -o $TMP_DIR/test || exit
$TMP_DIR/test        || exit

echo " Running 'Basic' example.."
gcc example/basic_tests.c \
    -o $TMP_DIR/test    || exit
$TMP_DIR/test           || exit

echo " Running 'Add' example.."
gcc example/add_test.c \
    example/suts/add.c \
    -o $TMP_DIR/test     || exit
$TMP_DIR/test            || exit

echo " Running 'Add Parameterised' example.."
gcc example/add_parameterised_test.c \
    example/suts/add.c               \
    -o $TMP_DIR/test || exit
$TMP_DIR/test        || exit

echo " Running 'Separate mock files' example.."
gcc example/separate_mock_files/dut_test.c \
    example/separate_mock_files/mock.c     \
    -o $TMP_DIR/test || exit
$TMP_DIR/test        || exit

echo "[ALl tests passed]"
