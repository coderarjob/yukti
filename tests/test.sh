#!/bin/bash

DIR=`dirname $0`
EXP_DIR="$DIR/exps"

if [ ! -e $EXP_DIR ]; then
    echo "Expectations folder not found. Exiting!"
    exit 1
fi

for fn in $DIR/*.c; do
    fname_we=${fn##*/}              # File title with extension
    fname=${fname_we%%.c}           # File title without extension

    bin_file=$DIR/test              # Compiled test binary name
    out_file=$DIR/test.out          # Output from running the test
    exp_file=$EXP_DIR/$fname.exp    # Expectation file for test

    echo "Compiling test '$fn'"
    gcc -Wall -Wextra $fn -o $bin_file || exit

    echo "Running and matching with expectation '$exp_file'"
    ./$bin_file > $out_file
    diff $out_file $exp_file || exit
done

echo "[Tests passed]"
