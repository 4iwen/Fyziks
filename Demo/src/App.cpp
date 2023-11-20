#include "App.h"

#include <imgui.h>

#include "Window.h"

using namespace fy;

void App::run() {
    // create the renderWindow
    auto window = Window("Fyziks Demo", sf::Vector2u(1440, 810));
    sf::Clock deltaClock;

    // define time step for the simulation
    float timeStep = 1.0f / 60.0f;
    // variable to store the elapsed time since last frame
    float elapsed = 0.0f;

    // world and objects
    World world;

    auto ground = world.create<Rectangle>(25.0f, 25.0f);
    ground->position = Vec2f(0, 0);

    auto rec1 = world.create<Rectangle>(75.0f, 50.0f);
    rec1->position = Vec2f(100, 0);

    auto cir1 = world.create<Circle>(25.0f);
    cir1->position = Vec2f(0, 100);

    auto cir2 = world.create<Circle>(35.0f);
    cir2->position = Vec2f(100, 100);

    auto pol1 = world.create<Polygon>(
            std::vector<Vec2f>{
                    Vec2f(-12.5f, -12.5f),
                    Vec2f(-12.5f, 12.5f),
                    Vec2f(12.5f, 12.5f),
                    Vec2f(12.5f, -12.5f)
            }
    );
    pol1->position = Vec2f(-100, -100);

    auto pol2 = world.create<Polygon>(
            std::vector<Vec2f>{
                    Vec2f(-30, -30),
                    Vec2f(-20, 0),
                    Vec2f(-30, 30),
                    Vec2f(30, 30),
                    Vec2f(20, 0),
                    Vec2f(30, -30)
            }
    );
    pol2->position = Vec2f(-100, 100);

    auto tri1 = world.create<Triangle>(
            Vec2f(0.0f, -35.0f),
            Vec2f(35.0f, 35.0f),
            Vec2f(-35.0f, 35.0f)
    );
    tri1->position = Vec2f(200, 100);

    auto tri2 = world.create<Triangle>(
            Vec2f(0.0f, -35.0f),
            Vec2f(35.0f, 35.0f),
            Vec2f(-35.0f, 35.0f)
    );
    tri2->position = Vec2f(100, -100);

    auto tri3 = world.create<Triangle>(
            Vec2f(0.0f, -35.0f),
            Vec2f(35.0f, 35.0f),
            Vec2f(-35.0f, 35.0f)
    );
    tri3->position = Vec2f(-100, 0);

    while (window.isOpen()) {
        // get the time passed since last frame
        sf::Time deltaTime = deltaClock.restart();

        // handle events (renderWindow, mouse, keyboard)
        window.handleEvents();
        window.updateCamera();

        // update imgui
        ImGui::SFML::Update(*window.renderWindow, deltaTime);

        // apply physics
        if (!paused) {
            // add elapsed time
            elapsed += deltaTime.asSeconds();

            while (elapsed >= timeStep) {
                world.step(timeStep);
                elapsed -= timeStep;
            }
        }

        // *-*-*-*-*-*-* draw to the buffer -*-*-*-*-*-*-*
        // clear the renderWindow
        window.clear(COLOR_GRAY);

        // draw imgui
        window.drawUI(&world, paused, timeStep);

        // draw world
        for (int i = 0; i < world.bodies.size(); ++i) {
            window.drawBody(world.bodies[i], i);
        }
        // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

        // render everything from the buffer
        window.render();
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
