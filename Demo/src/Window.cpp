#include "Window.h"

#include <cstdio>

Window::Window(std::string title, sf::Vector2u size) {
    this->pWindow = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(size),
            "Fyziks Demo"
    );

    if (!ImGui::SFML::Init(*pWindow)) {
        printf("Initializing ImGui with SFML failed!\n");
        return;
    }

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Window::clear(sf::Color color) {
    pWindow->clear(color);
}

void Window::render() {
    ImGui::SFML::Render(*pWindow);
    pWindow->display();
}

Window::~Window() {
    ImGui::SFML::Shutdown();
}

bool Window::isOpen() {
    return pWindow->isOpen();
}

void Window::handleEvents() {
    sf::Event event{};

    while (pWindow->pollEvent(event)) {
        ImGui::SFML::ProcessEvent(*pWindow, event);

        if (event.type == sf::Event::Closed)
            pWindow->close();
    }
}

void Window::drawUI() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::Begin("Metrics");
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::Text("Frame time: %.2f ms", 1000.0f / ImGui::GetIO().Framerate);
    static float values[90] = {0};
    static int values_offset = 0;
    values[values_offset] = 1000.0f / ImGui::GetIO().Framerate;
    values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
    ImGui::PlotLines("Frame\ntimes", values, IM_ARRAYSIZE(values), values_offset, nullptr, 0.0f, 100.0f,
                     ImVec2(0, 80));
    ImGui::End();

    ImGui::Begin("Examples");
    if (ImGui::TreeNode("Basic")) {
        if (ImGui::BeginTable("table1", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Basic example");
            ImGui::TableSetColumnIndex(1);
            ImGui::SmallButton("Load");
            ImGui::EndTable();
        }

        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Advanced")) {
        if (ImGui::BeginTable("table2", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Advanced example");
            ImGui::TableSetColumnIndex(1);
            ImGui::SmallButton("Load");
            ImGui::EndTable();
        }

        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Explosions")) {
        if (ImGui::BeginTable("table3", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Explosions example");
            ImGui::TableSetColumnIndex(1);
            ImGui::SmallButton("Load");
            ImGui::EndTable();
        }

        ImGui::TreePop();
    }
    ImGui::End();

    ImGui::Begin("Physics config");
    ImGui::Text("Physics config");
    ImGui::End();

    ImGui::ShowDemoWindow();
}