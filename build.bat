rmdir /s /q build
mkdir build
cd build
cmake -DFYZIKS_BUILD_DOCS=OFF ..
cmake --build . --config Release