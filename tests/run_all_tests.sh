#!/bin/bash

TESTS_DIR=`dirname $0`

#######################################################################
# Compiling and running tests with GCC (Native)
#######################################################################
export CC=gcc
export CFLAGS="-Wall -Wextra -std=c99 -DYT__TESTING"

echo "Test 1/2: Running tests with C Compiler ($CC) (Native)"
bash $TESTS_DIR/test.sh || exit

#######################################################################
# Compiling and running tests with GCC (m32)
#######################################################################
export CC=gcc
export CFLAGS="-Wall -Wextra -std=c99 -m32 -DYT__TESTING"

echo "Test 2/3: Running tests with C Compiler ($CC) (32-bits)"
bash $TESTS_DIR/test.sh || exit

#######################################################################
# Compiling and running tests with G++
#######################################################################
export CC=g++
export CFLAGS="-Wall -Wextra -DYT__TESTING"

echo "Test 3/3: Running tests with C++ Compiler ($CC)"
bash $TESTS_DIR/test.sh || exit

echo "[All tests passed]"
