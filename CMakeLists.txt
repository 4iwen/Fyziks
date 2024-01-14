cmake_minimum_required(VERSION 3.25)
project(Fyziks VERSION 1.0.0)

# cmake settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# compiler flags
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s -DNDEBUG")
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic")
endif ()

# options
option(FYZIKS_BUILD_DEMO "Build demo" ON)
option(FYZIKS_BUILD_SHARED "Build Fyziks as a shared library" OFF)

# library
add_subdirectory(Fyziks)


# demo
if (FYZIKS_BUILD_DEMO)
    # dependencies
    set(IMGUI_SFML_FIND_SFML OFF) # don't find SFML
    set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui) # specify imgui directory instead of finding it
    add_subdirectory(vendor/imgui-sfml)

    set(BUILD_SHARED_LIBS FALSE) # don't build shared libraries -> static libraries
    set(SFML_BUILD_AUDIO FALSE) # don't build audio module
    set(SFML_BUILD_NETWORK FALSE) # don't build network module
    add_subdirectory(vendor/SFML)

    add_subdirectory(Demo)
endif ()
