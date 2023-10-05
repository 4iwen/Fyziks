#include "App.h"

void App::run() {
    // create the window
    std::unique_ptr<Window> window = std::make_unique<Window>("Fyziks Demo", sf::Vector2u(1440, 720));
    sf::Clock deltaClock;
    window->setVsync(true);

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

        // tester for rotation
        static float rot = 0.0f;
        ImGui::Begin("tester");
        ImGui::SliderAngle("rot", &rot, 0.0f, 360.0f);
        ImGui::End();

        //                 width,  height,position,                 rotation (radians)
        fy::Rectangle rec1(120.0f, 80.0f, fy::Vec2(720.0f, 230.0f), rot);
        window->drawRectangle(&rec1);

        //                vertices,                 position,                 rotation
        fy::Triangle tri1(fy::Vec2(-100.0f, -50.0f),
                          fy::Vec2(0.0f, 100.0f),
                          fy::Vec2(100.0f, -50.0f), fy::Vec2(800.0f, 400.0f), rot);
        window->drawTriangle(&tri1);

        //              radius,position                  rotation
        fy::Circle cir1(50.0f, fy::Vec2(600.0f, 200.0f), rot);
        window->drawCircle(&cir1);

        //                vertices,                 position                  rotation
        fy::Polygon pol1({fy::Vec2(-50.0f, -60.0f),
                          fy::Vec2(-10.0f, 40.0f),
                          fy::Vec2(20.0f, 35.0f),
                          fy::Vec2(35.0f, 0.0f),
                          fy::Vec2(25.0f, -15.0f)}, fy::Vec2(660.0f, 550.0f), rot);
        window->drawPolygon(&pol1);
        // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

        // render everything from the buffer
        window->render();
    }
}
