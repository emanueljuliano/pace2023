#!/bin/bash

TEST_CASES="./tests/exact-public/*"
PROGRAM="./build/pace"

N=25

function run_test() {
	timeout 30m $PROGRAM < $f &> /dev/null;
	if [ $? != 124 ]; then
		echo $1
	fi
}

for f in $TEST_CASES
do
	((i=i%N)); ((i++==0)) && wait
	run_test $f &
done
