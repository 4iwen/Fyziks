cmake_minimum_required(VERSION 3.25)
project(Fyziks VERSION 0.1.0)

# cmake settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

# options
option(FYZIKS_BUILD_DEMO "Build Demo" ON)

# include directories
include_directories(Fyziks/include)

# targets
add_subdirectory(Fyziks)
if (FYZIKS_BUILD_DEMO)
    # dependencies
    set(IMGUI_DIR vendor/imgui)
    set(IMGUI_SFML_FIND_SFML OFF)

    add_subdirectory(vendor/imgui-sfml)
    add_subdirectory(vendor/SFML)

    # demo
    add_subdirectory(Demo)
endif ()