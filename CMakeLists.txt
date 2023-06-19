cmake_minimum_required(VERSION 3.25)
project(Fyziks VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

option(FYZIKS_BUILD_DEMO "Build Demo" ON)

set(FYZIKS_INCLUDE_DIR Fyziks/include)
include_directories(${FYZIKS_INCLUDE_DIR})

# targets
add_subdirectory(Fyziks)
if (FYZIKS_BUILD_DEMO)
    add_subdirectory(Demo)
endif ()

# dependencies