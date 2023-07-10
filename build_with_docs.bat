rmdir /s /q build
mkdir build
cd build
cmake -DFYZIKS_BUILD_DOCS=ON ..
cmake --build . --config Release