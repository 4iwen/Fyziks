cmake_minimum_required(VERSION 3.25)
project(Fyziks VERSION 0.1.0)

# cmake settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# options
option(FYZIKS_BUILD_DEMO "Build demo" ON)
option(FYZIKS_BUILD_DOCS "Build documentation" OFF)
option(FYZIKS_BUILD_SHARED "Build Fyziks as a shared library" ON)

# library
add_subdirectory(Fyziks)

# demo
if (FYZIKS_BUILD_DEMO)
    # dependencies
    set(IMGUI_SFML_FIND_SFML OFF)
    set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)

    add_subdirectory(vendor/imgui-sfml)
    add_subdirectory(vendor/SFML)

    add_subdirectory(Demo)
endif ()

# docs
if (FYZIKS_BUILD_DOCS)
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        message(STATUS "============ Doxygen found, building documentation ============")
        add_subdirectory(docs)
    endif ()
endif ()
