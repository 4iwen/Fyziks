#include "App.h"

void App::init() {
    this->window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(sf::Vector2u(1080, 640)),
            "Fyziks Demo"
    );
}

void App::run() {
    if (!ImGui::SFML::Init(*window)) {
        printf("Initializing ImGui with SFML failed!\n");
        return;
    }

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    sf::Clock deltaClock;
    while (window->isOpen()) {
        sf::Event event{};
        while (window->pollEvent(event)) {
            ImGui::SFML::ProcessEvent(*window, event);

            if (event.type == sf::Event::Closed)
                window->close();
        }


        ImGui::SFML::Update(*window, deltaClock.restart());

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::ShowDemoWindow();

        window->clear(sf::Color(33, 33, 33));

        ImGui::SFML::Render(*window);

        window->display();
    }

    ImGui::SFML::Shutdown();
}