#include "Window.h"

#include <cstdio>
#include <cmath>

using namespace fy;

sf::Color defaultColor = COLOR_WHITE;
sf::Color collisionColor = COLOR_RED;

Window::Window(std::string title, sf::Vector2u size) {
    this->window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(size.x, size.y), title, sf::Style::Default,
            sf::ContextSettings(0, 0, 8));
    this->view = std::make_unique<sf::View>(sf::FloatRect(0, 0, (float) size.x, (float) size.y));
    window->setView(*view);

    if (!ImGui::SFML::Init(*window)) {
        printf("Initializing ImGui with SFML failed!\n");
        return;
    }

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Window::clear(sf::Color color) { window->clear(color); }

void Window::render() {
    ImGui::SFML::Render(*window);
    window->display();
}

Window::~Window() { ImGui::SFML::Shutdown(); }

bool Window::isOpen() { return window->isOpen(); }

void Window::handleEvents() {
    sf::Event event{};

    while (window->pollEvent(event)) {
        ImGui::SFML::ProcessEvent(*window, event);

        if (event.type == sf::Event::Closed) {
            window->close();
        }
        if (event.type == sf::Event::Resized) {
            sf::Vector2f viewCenter = view->getCenter();
            view->setSize((float) event.size.width, (float) event.size.height);
            view->setCenter(viewCenter);
            window->setView(*view);
        }
        if (event.type == sf::Event::MouseWheelScrolled) {
            view->zoom(1.0f + -event.mouseWheelScroll.delta * 0.1f);
            window->setView(*view);
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            lastMousePosition = sf::Mouse::getPosition();
        }
    }
}

void Window::updateCamera() {
    // when right mouse button is pressed calculate the new camera position and update it
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2i newMousePosition = sf::Mouse::getPosition();
        sf::Vector2f offset = window->mapPixelToCoords(newMousePosition) - window->mapPixelToCoords(lastMousePosition);
        view->move(-offset);
        window->setView(*view);
        lastMousePosition = newMousePosition;
    }
}

void Window::setVsync(bool enabled) {
    window->setVerticalSyncEnabled(enabled);
}

void Window::drawUI(World *world) {
    // make the whole window dockable
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(),
                                 ImGuiDockNodeFlags_PassthruCentralNode);
    // metrics window
    drawMetrics();
    // demos window
    drawDemos();
    // physics config
    drawPhysicsConfig(world);
    // objects
    drawObjectConfig(world);
    // demo window
    // ImGui::ShowDemoWindow();
}

void Window::drawRectangle(const Rectangle *rectangle) const {
    std::vector<Vec2f> translatedVertices = rectangle->getTranslatedVertices();

    // calculate the 4 corners of the rectangle
    Vec2f v1 = translatedVertices[0];
    Vec2f v2 = translatedVertices[1];
    Vec2f v3 = translatedVertices[2];
    Vec2f v4 = translatedVertices[3];

    sf::Color color;
    if (rectangle->colliding) {
        color = collisionColor;
    } else {
        color = defaultColor;
    }

    // add the to vertex array
    sf::Vertex vertices[5] = {
            sf::Vertex(sf::Vector2f(v1.x, v1.y), color),
            sf::Vertex(sf::Vector2f(v2.x, v2.y), color),
            sf::Vertex(sf::Vector2f(v3.x, v3.y), color),
            sf::Vertex(sf::Vector2f(v4.x, v4.y), color),
            sf::Vertex(sf::Vector2f(v1.x, v1.y), color)
    };

    // draw rectangle
    window->draw(vertices, 5, sf::LineStrip);
}

void Window::drawCircle(const Circle *circle) const {
    // segments of the circle
    const int segments = 18;
    sf::Vertex vertices[segments + 2];
    Mat2x2 rotation(circle->rotation);
    const float angleIncrement = 2 * PIf / static_cast<float>(segments);

    sf::Color color;
    if (circle->colliding) {
        color = collisionColor;
    } else {
        color = defaultColor;
    }

    // fill the vertex array
    for (int i = 0; i < segments + 1; i++) {
        float angle = static_cast<float>(i) * angleIncrement;
        Vec2f point =
                circle->position + rotation * Vec2f(circle->radius * cosf(angle), circle->radius * sinf(angle));
        vertices[i] = sf::Vertex(sf::Vector2f(point.x, point.y), color);
    }
    vertices[segments + 1] = sf::Vertex(sf::Vector2f(circle->position.x, circle->position.y), color);

    // draw the circle
    window->draw(vertices, segments + 2, sf::LineStrip);
}

void Window::drawTriangle(const Triangle *triangle) const {
    std::vector<Vec2f> translatedVertices = triangle->getTranslatedVertices();


    // get the 3 points of a triangle
    Vec2f v1 = translatedVertices[0];
    Vec2f v2 = translatedVertices[1];
    Vec2f v3 = translatedVertices[2];

    sf::Color color;
    if (triangle->colliding) {
        color = collisionColor;
    } else {
        color = defaultColor;
    }

    // create a vertex array
    sf::Vertex vertices[4] = {
            sf::Vertex(sf::Vector2f(v1.x, v1.y), color),
            sf::Vertex(sf::Vector2f(v2.x, v2.y), color),
            sf::Vertex(sf::Vector2f(v3.x, v3.y), color),
            sf::Vertex(sf::Vector2f(v1.x, v1.y), color)
    };

    // draw the triangle
    window->draw(vertices, 4, sf::LineStrip);
}

void Window::drawPolygon(const Polygon *polygon) const {
    std::vector<Vec2f> translatedVertices = polygon->getTranslatedVertices();

    unsigned long verticesCount = translatedVertices.size();

    sf::Vertex* vertices = new sf::Vertex[verticesCount + 1];

    sf::Color color;
    if (polygon->colliding) {
        color = collisionColor;
    } else {
        color = defaultColor;
    }

    for (int i = 0; i < verticesCount; i++) {
        Vec2f point = translatedVertices[i];
        vertices[i] = sf::Vertex(sf::Vector2f(point.x, point.y), color);
    }
    vertices[verticesCount] = vertices[0];

    window->draw(vertices, verticesCount + 1, sf::LineStrip);

    delete[] vertices;
}

void Window::drawShape(const Body *shape) const {
    if (const auto *rectangle = dynamic_cast<const Rectangle *>(shape)) {
        drawRectangle(rectangle);
    } else if (const auto *circle = dynamic_cast<const Circle *>(shape)) {
        drawCircle(circle);
    } else if (const auto *triangle = dynamic_cast<const Triangle *>(shape)) {
        drawTriangle(triangle);
    } else if (const auto *polygon = dynamic_cast<const Polygon *>(shape)) {
        drawPolygon(polygon);
    }
}

void Window::drawMetrics() {
    ImGui::Begin("Metrics"); // begins a new window with a name
    ImGui::Text("FPS: %.2f",
                ImGui::GetIO().Framerate); // displays the fps as a text
    ImGui::Text("Frame time: %.2f ms",
                1000.0f /
                ImGui::GetIO().Framerate); // displays the frame times as text
    static float values[90] = {0};
    static int values_offset = 0;
    values[values_offset] = 1000.0f / ImGui::GetIO().Framerate;
    values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
    ImGui::PlotLines("Frame\ntimes", values, IM_ARRAYSIZE(values), values_offset,
                     nullptr, 0.0f, 100.0f,
                     ImVec2(0, 80)); // displays the frame times as a chart
    ImGui::End();                    // ends the window
}

void Window::drawDemos() {
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
}

void Window::drawPhysicsConfig(World *world) {
    ImGui::Begin("Physics config");

    ImGui::SliderInt("Iterations", &world->iterations, 1, 32);

    static int steps = 60;
    ImGui::SliderInt("Time step", &steps, 1, 120);
    world->timeStep = 1.0f / (float) steps;

    ImGui::DragFloat2("Gravity", &world->gravity.x);

    ImGui::End();
}

void Window::drawObjectConfig(World *world) {
    ImGui::Begin("Objects");

    for (int i = 0; i < world->bodies.size(); ++i) {
        Body *body = world->bodies[i];

        std::string name = "obj [" + std::to_string(i) + "]";

        ImGui::PushID(i);
        if (ImGui::TreeNode(name.c_str())) {
            ImGui::DragFloat2("Position", &body->position.x);
            ImGui::SliderAngle("Rotation", &body->rotation, 0.0f, 360.0f);
            ImGui::DragFloat2("Velocity", &body->velocity.x);
            ImGui::DragFloat("Angular velocity", &body->angularVelocity);

            ImGui::Spacing();

            ImGui::DragFloat("Mass", &body->mass);
            ImGui::DragFloat("Inertia", &body->inertia);
            ImGui::DragFloat("Friction", &body->friction);

            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    ImGui::End();
}
