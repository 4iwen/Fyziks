#include "App.h"

#include "Window.h"
#include "Renderer.h"
#include "BodyProperties.h"

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

    BodyProperties bodyProps;

    while (window.isOpen()) {
        // get the time passed since last frame
        sf::Time deltaTime = deltaClock.restart();

        // update imgui
        ImGui::SFML::Update(*window.renderWindow, deltaTime);

        // handle events (window, mouse, keyboard)
        window.handleEvents(&world, &bodyProps);
        window.updateCamera();

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
        renderer.drawUI(&world, &bodyProps, paused, timeStep);

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
        case 3:
            demo4(world);
            break;
        case 4:
            demo5(world);
            break;
    }
}

void App::demo1(World *world) {
    world->gravity = Vec2f(0, 196.2f);

    auto ground = world->create<Rectangle>(1500.0f, 50.0f);
    ground->position = Vec2f(0, 350);
    ground->restitution = 1;
    ground->isStatic = true;

    world->create<Rectangle>(50, 50)->position = Vec2f(-300, 0);

    world->create<Circle>(25)->position = Vec2f(-150, 0);

    world->create<Triangle>(
            Vec2f(-35.0f, 35.0f),
            Vec2f(35.0f, 35.0f),
            Vec2f(0.0f, -35.0f)
    )->position = Vec2f(0, 0);

    world->create<Polygon>(std::vector{
            Vec2f(-30, 10),
            Vec2f(-20, 25),
            Vec2f(-5, 15),
            Vec2f(15, 20),
            Vec2f(10, 0),
            Vec2f(25, -15),
            Vec2f(20, -30),
            Vec2f(0, -20),
            Vec2f(-15, -10),
            Vec2f(-25, -5)
            })->position = Vec2f(150, 0);

    world->create<Triangle>(
            Vec2f(-21.65, 12.5),
            Vec2f(21.65, 12.5),
            Vec2f(0, -25)
    )->position = Vec2f(300, 0);
}

void App::demo2(World *world) {
    world->gravity = Vec2f(0, 196.2f);

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
    world->gravity = Vec2f(0, 196.2f);

    auto ground = world->create<Rectangle>(750.0f, 50.0f);
    ground->position = Vec2f(0, 50);
    ground->restitution = 0;
    ground->isStatic = true;
    ground->dynamicFriction = 0;
    ground->staticFriction = 0;

    auto ramp = world->create<Rectangle>(500.0f, 20.0f);
    ramp->position = Vec2f(-150, -10);
    ramp->rotation = Misc::toRadians(15);
    ramp->isStatic = true;
    ramp->dynamicFriction = 0;
    ramp->staticFriction = 0;

    for (int i = 0; i < 6; ++i) {
        auto rect = world->create<Rectangle>(25, 25);
        rect->position = Vec2f(-280 + i * 50, -80 + i * 15);
        rect->dynamicFriction = 1.0f - i * 0.2f;
        rect->staticFriction = 1.0f - i * 0.2f;
    }
}

void App::demo4(World *world) {
    world->gravity = Vec2f(0, 0);

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
}

void App::demo5(World *world) {
    world->gravity = Vec2f(0, 196.2f);

    auto ground = world->create<Rectangle>(750.0f, 50.0f);
    ground->position = Vec2f(0, 350);
    ground->restitution = 1;
    ground->isStatic = true;

    for (int i = 0; i < 11; ++i) {
        auto circle = world->create<Circle>(25);
        circle->position = Vec2f(-300 + i * 60, 0);
        circle->restitution = i * 0.1f;
    }
}