set(
        SOURCE_FILES
        src/FyMath.cpp
)

include_directories(include)

if (FYZIKS_BUILD_SHARED)
    add_compile_definitions(FYZIKS_SHARED FYZIKS_EXPORTS)
    add_library(Fyziks SHARED ${SOURCE_FILES})
else ()
    add_compile_definitions(FYZIKS_EXPORTS)
    add_library(Fyziks STATIC ${SOURCE_FILES})
endif ()