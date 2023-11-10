#include "App.h"

using namespace fy;

void App::run() {
    // create the window
    std::unique_ptr<Window> window = std::make_unique<Window>("Fyziks Demo", sf::Vector2u(1440, 810));
    sf::Clock deltaClock;
    window->setVsync(true);

    World world;

    Circle cir1(25.0f);
    cir1.position = Vec2f(700, 300);
    world.add(&cir1);

    Circle cir2(35.0f);
    cir2.position = Vec2f(700, 330);
    world.add(&cir2);

    Rectangle rec1(50.0f, 50.0f);
    rec1.position = Vec2f(720, 460);
    world.add(&rec1);

    Triangle tri1(Vec2f(0.0f, -35.0f), Vec2f(35.0f, 35.0f), Vec2f(-35.0f, 35.0f));
    tri1.position = Vec2f(820, 360);
    world.add(&tri1);

    Polygon pol1({Vec2f(-15.0f, -15.0f), Vec2f(-30.0f, 30.0f), Vec2f(15.0f, 15.0f), Vec2f(30.0f, -30.0f)});
    pol1.position = Vec2f(820, 360);
    world.add(&pol1);

    Polygon pol2({Vec2f(-30, -30), Vec2f(-20, 0), Vec2f(-30, 30), Vec2f(30, 30), Vec2f(20, 0), Vec2f(30, -30)});
    pol2.position = Vec2f(820, 460);
    world.add(&pol2);

    Triangle tri2(Vec2f(0.0f, -35.0f), Vec2f(35.0f, 35.0f), Vec2f(-35.0f, 35.0f));
    tri2.position = Vec2f(720, 560);
    world.add(&tri2);

    Triangle tri3(Vec2f(0.0f, -35.0f), Vec2f(35.0f, 35.0f), Vec2f(-35.0f, 35.0f));
    tri3.position = Vec2f(820, 560);
    world.add(&tri3);

    while (window->isOpen()) {
        // handle events (window, mouse, keyboard)
        window->handleEvents();
        window->updateCamera();
        // clear the window
        window->clear(COLOR_GRAY);

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