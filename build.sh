#!/bin/bash

rm -rf build
mkdir build
cd build
cmake -DFYZIKS_BUILD_DEMO=ON -DFYZIKS_BUILD_SHARED=ON ..
cmake --build . --config Release
