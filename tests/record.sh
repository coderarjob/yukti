#!/bin/bash

DIR=`dirname $0`
EXP_DIR="$DIR/exps"

rm -rf $EXP_DIR             || exit

echo "Creating directory '$EXP_DIR'"
mkdir $EXP_DIR              || exit

for fn in $DIR/*.c; do
    fname_we=${fn##*/}              # File title with extension
    fname=${fname_we%%.c}           # File title without extension

    bin_file=$DIR/test              # Compiled test binary name
    exp_file=$EXP_DIR/$fname.exp    # Expectation file for test

    echo "Compiling test '$fn'"
    gcc -Wall -Wextra $fn -o $bin_file || exit

    echo "Running and recording output into '$exp_file'"
    ./$bin_file > $exp_file
    chmod -w $exp_file      || exit
done

echo "[Recording finished]"
