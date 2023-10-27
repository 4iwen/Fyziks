#include "App.h"

using namespace fy;

void App::run() {
    // create the window
    std::unique_ptr<Window> window = std::make_unique<Window>("Fyziks Demo", sf::Vector2u(1440, 720));
    sf::Clock deltaClock;
    window->setVsync(true);

    World world;
    Rectangle rec1(50.0f, 50.0f);
    rec1.position = Vec2(720, 360);
    rec1.mass = 10.0f;
    rec1.torque = 1000.0f;
    rec1.inertia = 10.0f;
    world.add(&rec1);

    Rectangle rec2(50.0f, 50.0f);
    rec2.position = Vec2(820, 360);
    rec2.mass = 1000.0f;
    rec2.torque = 100.0f;
    rec2.inertia = 100.0f;
    world.add(&rec2);

    while (window->isOpen()) {
        // handle events (window, mouse, keyboard)
        window->handleEvents();
        window->updateCamera();
        // clear the window
        window->clear(window->colors[6]);

        // update imgui
        ImGui::SFML::Update(*window->window, deltaClock.restart());

        // apply physics
        world.step();

        // *-*-*-*-*-*-* draw to the buffer -*-*-*-*-*-*-*
        // draw imgui
        window->drawUI(&world);

        // draw world
        for (int i = 0; i < world.bodies.size(); ++i) {
            window->drawShape(world.bodies[i]);
        }
        // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

        // render everything from the buffer
        window->render();
    }
}

void App::demo1() {

}

void App::demo2() {

}

void App::demo3() {

}

void App::runDemo(int number) {
    switch (number) {
        default:
        case 1:
            demo1();
            break;
        case 2:
            demo2();
            break;
        case 3:
            demo3();
            break;
    }
}