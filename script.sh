#!/bin/bash

TEST_CASES="./tests/exact-public/*"
PROGRAM="./build/pace"

N=34

function kaio() {
	timeout 10m $PROGRAM < $f &> /dev/null;
	if [ $? != 124 ]; then
		echo $1
	fi
}

for f in $TEST_CASES
do
	((i=i%N)); ((i++==0)) && wait
	kaio $f &
done
