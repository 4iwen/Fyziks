rmdir /s /q build
mkdir build
cd build
cmake -G "MinGW Makefiles" -DFYZIKS_BUILD_DEMO=ON -DFYZIKS_BUILD_SHARED=ON ..
cmake --build . --config Release
