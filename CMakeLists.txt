cmake_minimum_required(VERSION 3.25)
project(Fyziks VERSION 1.0.0)

# cmake settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# options
option(FYZIKS_BUILD_DEMO "Build demo" ON)
option(FYZIKS_BUILD_SHARED "Build Fyziks as a shared library" OFF)

# library
add_subdirectory(Fyziks)

# demo
if (FYZIKS_BUILD_DEMO)
    # dependencies
    find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)
    set(SFML_BUILD_AUDIO FALSE)
    set(SFML_BUILD_NETWORK FALSE)

    set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
    add_subdirectory(vendor/imgui-sfml)

    add_subdirectory(Demo)
endif ()
