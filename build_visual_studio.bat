rmdir /s /q build
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
