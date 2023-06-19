set(SOURCE_FILES
        src/Fyziks.cpp include/Fyziks/Core.h)

add_compile_definitions(FYZIKS_EXPORTS)

add_library(Fyziks SHARED
        ${SOURCE_FILES})