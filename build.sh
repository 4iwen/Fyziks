#!/bin/bash

rm -rf build
mkdir build
cd build
cmake -DFYZIKS_BUILD_DEMO=ON -DFYZIKS_BUILD_SHARED=OFF -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
