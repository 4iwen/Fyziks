set(
        SOURCE_FILES
        src/Physics/World.cpp
        src/Math/Body.cpp
        src/Physics/Collision.cpp
        src/Math/PolygonHelper.cpp
)

include_directories(include)

if (FYZIKS_BUILD_SHARED)
    add_library(Fyziks SHARED ${SOURCE_FILES})
    target_compile_definitions(Fyziks PRIVATE FYZIKS_SHARED FYZIKS_EXPORTS)
else ()
    add_library(Fyziks STATIC ${SOURCE_FILES})
    target_compile_definitions(Fyziks PRIVATE FYZIKS_EXPORTS)
endif ()
