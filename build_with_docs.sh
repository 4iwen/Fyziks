rm -rf build
mkdir build
cd build
cmake -DFYZIKS_BUILD_DOCS=ON ..
cmake --build . --config Release