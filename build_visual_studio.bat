rmdir /s /q build
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -DFYZIKS_BUILD_DEMO=ON -DFYZIKS_BUILD_SHARED=ON ..
cmake --build . --config Release
