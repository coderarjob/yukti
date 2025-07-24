#!/bin/bash

TESTS_DIR=`dirname $0`

#######################################################################
# Compiling and running tests with GCC
#######################################################################
export CC=gcc
export CFLAGS="-Wall -Wextra -std=c99"

echo "Test 1/2: Running tests with C Compiler ($CC)"
bash $TESTS_DIR/test.sh || exit

#######################################################################
# Compiling and running tests with G++
#######################################################################
export CC=g++
export CFLAGS="-Wall -Wextra"

echo "Test 2/2: Running tests with C++ Compiler ($CC)"
bash $TESTS_DIR/test.sh || exit

echo "[ALl tests passed]"
