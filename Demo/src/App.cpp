#include "App.h"

#include "Window.h"
#include "Renderer.h"

using namespace fy;

void App::run() {
    // create the window
    auto window = Window("Fyziks Demo", sf::Vector2u(1440, 810));
    auto renderer = Renderer(window.renderWindow);
    sf::Clock deltaClock;

    // define time step for the simulation
    float timeStep = 1.0f / 60.0f;
    // variable to store the elapsed time since last frame
    float elapsed = 0.0f;

    // world and objects
    World world;
    demo1(&world);

    sf::Clock performance;

    while (window.isOpen()) {
        // get the time passed since last frame
        sf::Time deltaTime = deltaClock.restart();

        // handle events (window, mouse, keyboard)
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
        // clear the window
        window.clear(COLOR_GRAY);

        // draw imgui
        renderer.drawUI(&world, paused, timeStep);

        // draw world
        renderer.drawWorld(&world);
        // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

        // render everything from the buffer
        window.render();
    }
}

void App::loadDemo(int i, World *world) {
    world->clear();

    switch (i) {
        default:
        case 0:
            demo1(world);
            break;
        case 1:
            demo2(world);
            break;
        case 2:
            demo3(world);
            break;
    }
}

void App::demo1(World *world) {
    auto ground = world->create<Rectangle>(750.0f, 50.0f);
    ground->position = Vec2f(0, 350);
    ground->restitution = 1;
    ground->isStatic = true;

    auto rectangle = world->create<Rectangle>(50, 50);
    rectangle->rotation = 1;
    rectangle->mass = 1;
    rectangle->position = Vec2f(-100, 0);

    auto circle = world->create<Circle>(25);
    circle->mass = 1;
    circle->position = Vec2f(0, 0);

    auto triangle = world->create<Triangle>(
            Vec2f(0.0f, 35.0f),
            Vec2f(-35.0f, -35.0f),
            Vec2f(35.0f, -35.0f)
    );
    triangle->mass = 1;
    triangle->position = Vec2f(100, 0);
}

void App::demo2(World *world) {
    auto ground = world->create<Rectangle>(750.0f, 50.0f);
    ground->position = Vec2f(0, 350);
    ground->restitution = 1;
    ground->isStatic = true;

    auto ground1 = world->create<Rectangle>(750.0f, 50.0f);
    ground1->position = Vec2f(0, -350);
    ground1->restitution = 1;
    ground1->isStatic = true;

    auto wall = world->create<Rectangle>(50.0f, 750.0f);
    wall->position = Vec2f(350, 0);
    wall->restitution = 1;
    wall->isStatic = true;

    auto wall1 = world->create<Rectangle>(50.0f, 750.0f);
    wall1->position = Vec2f(-350, 0);
    wall1->restitution = 1;
    wall1->isStatic = true;

    for (int i = 0; i < 10; ++i) {
        world->create<Circle>(25);
    }

    for (int i = 0; i < 10; ++i) {
        world->create<Rectangle>(25, 25);
    }
}

void App::demo3(World *world) {

}
