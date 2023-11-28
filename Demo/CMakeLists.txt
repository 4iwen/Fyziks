set(
        SOURCE_FILES

        src/Demo.cpp
        src/App.cpp
        src/Window.cpp
        src/Renderer.cpp
)

set(
        IMGUI_SOURCE_FILES

        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ../vendor/imgui-sfml/imgui-SFML.cpp
)

include_directories(
        ${IMGUI_DIR}
        ../Fyziks/include
        ../vendor/SFML/include
        ../vendor/imgui-sfml
)

add_executable(
        Demo

        ${SOURCE_FILES}
        ${IMGUI_SOURCE_FILES}
)

target_link_libraries(
        Demo PRIVATE

        Fyziks
        sfml-graphics
        sfml-window
        sfml-system
        ImGui-SFML::ImGui-SFML
)