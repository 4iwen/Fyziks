set(SOURCE_FILES
        src/Demo.cpp)

add_executable(Demo
        ${SOURCE_FILES})

target_link_libraries(Demo PUBLIC
        Fyziks
        ImGui-SFML::ImGui-SFML)