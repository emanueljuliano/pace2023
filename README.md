https://pacechallenge.org/2023/
# 2023 PACE Challenge
This repository provides an exact solver to contract a given Graph into a single vertex.
We approach this problem by implementing the SAT formulation for it, combined with an algorithm which finds a modular-like decomposition of a graph and a tree recognition method.

## Build Application 
1- Create a build directory: 
`mkdir build && cd build`

2- Run cmake: `cmake`

3- Run make: `make..`

## Run Tests
To run the tests, pass the name of a test file, as shown below (from build folder):

`./pace < <test file>`

## Requirements
- A 64-bit Linux operating system.
- A c++ 17 compiler, preferably GCC.
- The cmake build system(>=3.5.1).
