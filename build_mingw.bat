rmdir /s /q build
mkdir build
cd build
cmake -G "MinGW Makefiles" -DFYZIKS_BUILD_DEMO=ON -DFYZIKS_BUILD_SHARED=OFF -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
