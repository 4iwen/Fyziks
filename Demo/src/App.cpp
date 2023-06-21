#include "App.h"

void App::run() {
    std::unique_ptr<Window> window = std::make_unique<Window>("Fyziks Demo", sf::Vector2u(1440, 720));
    sf::Clock deltaClock;

    while (window->isOpen()) {
        // handle events
        window->handleEvents();

        // *-*-*-*-*-*-* draw -*-*-*-*-*-*-*
        ImGui::SFML::Update(*window->pWindow, deltaClock.restart());

        window->drawUI();
        // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

        // render
        window->clear(sf::Color(33, 33, 33));
        window->render();
    }
}