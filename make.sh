#!/bin/bash
mkdir build
cd build || exit
cmake -D CMAKE_BUILD_TYPE=Release ..
make
cd ..
