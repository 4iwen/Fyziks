rm -rf build
mkdir build
cd build
cmake -DFYZIKS_BUILD_DOCS=OFF ..
cmake --build . --config Release