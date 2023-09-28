#include "App.h"

void App::run() {
    // create the window
    std::unique_ptr<Window> window =
            std::make_unique<Window>("Fyziks Demo", sf::Vector2u(1440, 720));
    sf::Clock deltaClock;

    while (window->isOpen()) {
        // handle events (window, mouse, keyboard)
        window->handleEvents();
        // clear the window
        window->clear(window->colors[7]);

        // update imgui
        ImGui::SFML::Update(*window->window, deltaClock.restart());

        // *-*-*-*-*-*-* draw to the buffer -*-*-*-*-*-*-*
        // draw imgui
        window->drawUI();

        // draw rectangles
        //                width,  height, position,                 rotation (radians)
        fy::Rectangle rec(100.0f, 100.0f, fy::Vec2(600.0f, 350.0f), 6.0f);
        window->drawRectangle(&rec);
        fy::Rectangle rec1(120.0f, 80.0f, fy::Vec2(720.0f, 230.0f), 1.0f);
        window->drawRectangle(&rec1);
        // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

        // render everything from the buffer
        window->render();
    }
}
