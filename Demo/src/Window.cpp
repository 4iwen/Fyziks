#include "Window.h"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/System/Vector2.hpp"

#include <cstdio>
#include <cmath>

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
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            lastMousePosition = sf::Mouse::getPosition();
        }
    }
}

void Window::updateCamera() {
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

void Window::drawUI(fy::World *world) {
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
}

void Window::drawRectangle(const fy::Rectangle *rectangle) const {
    fy::Mat2x2 rotation(rectangle->rotation);
    fy::Vec2 half(rectangle->width * 0.5f, rectangle->height * 0.5f);

    fy::Vec2 v1 = rectangle->position + rotation * fy::Vec2(-half.x, -half.y);
    fy::Vec2 v2 = rectangle->position + rotation * fy::Vec2(half.x, -half.y);
    fy::Vec2 v3 = rectangle->position + rotation * fy::Vec2(half.x, half.y);
    fy::Vec2 v4 = rectangle->position + rotation * fy::Vec2(-half.x, half.y);

    sf::Vertex vertices[5] = {
            sf::Vertex(sf::Vector2f(v1.x, v1.y), colors[5]),
            sf::Vertex(sf::Vector2f(v2.x, v2.y), colors[5]),
            sf::Vertex(sf::Vector2f(v3.x, v3.y), colors[5]),
            sf::Vertex(sf::Vector2f(v4.x, v4.y), colors[5]),
            sf::Vertex(sf::Vector2f(v1.x, v1.y), colors[5])
    };

    window->draw(vertices, 5, sf::LineStrip);
}

void Window::drawCircle(const fy::Circle *circle) const {
    int segments = 18;
    sf::Vertex vertices[segments + 2];
    fy::Mat2x2 rotation(circle->rotation);
    const float angleIncrement = 2 * M_PIf / static_cast<float>(segments);

    for (int i = 0; i < segments + 1; i++) {
        float angle = static_cast<float>(i) * angleIncrement;
        fy::Vec2 point =
                circle->position + rotation * fy::Vec2(circle->radius * cosf(angle), circle->radius * sinf(angle));
        vertices[i] = sf::Vertex(sf::Vector2f(point.x, point.y), colors[5]);
    }
    vertices[segments + 1] = sf::Vertex(sf::Vector2f(circle->position.x, circle->position.y), colors[5]);


    window->draw(vertices, segments + 2, sf::LineStrip);
}

void Window::drawTriangle(const fy::Triangle *triangle) const {
    fy::Mat2x2 rotation(triangle->rotation);

    fy::Vec2 v1 = triangle->position + rotation * triangle->point1;
    fy::Vec2 v2 = triangle->position + rotation * triangle->point2;
    fy::Vec2 v3 = triangle->position + rotation * triangle->point3;

    sf::Vertex vertices[4] = {
            sf::Vertex(sf::Vector2f(v1.x, v1.y), colors[5]),
            sf::Vertex(sf::Vector2f(v2.x, v2.y), colors[5]),
            sf::Vertex(sf::Vector2f(v3.x, v3.y), colors[5]),
            sf::Vertex(sf::Vector2f(v1.x, v1.y), colors[5])
    };

    window->draw(vertices, 4, sf::LineStrip);
}

void Window::drawPolygon(const fy::Polygon *polygon) const {
    unsigned long verticesCount = polygon->vertices.size();

    sf::Vertex vertices[verticesCount + 1];
    fy::Mat2x2 rotation(polygon->rotation);

    for (int i = 0; i < verticesCount; i++) {
        fy::Vec2 point = polygon->position + rotation * polygon->vertices[i];
        vertices[i] = sf::Vertex(sf::Vector2f(point.x, point.y), colors[5]);
    }
    vertices[verticesCount] = vertices[0];

    window->draw(vertices, verticesCount + 1, sf::LineStrip);
}

void Window::drawShape(const fy::Body *shape) const {
    if (const auto *rectangle = dynamic_cast<const fy::Rectangle *>(shape)) {
        drawRectangle(rectangle);
    } else if (const auto *circle = dynamic_cast<const fy::Circle *>(shape)) {
        drawCircle(circle);
    } else if (const auto *triangle = dynamic_cast<const fy::Triangle *>(shape)) {
        drawTriangle(triangle);
    } else if (const auto *polygon = dynamic_cast<const fy::Polygon *>(shape)) {
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

void Window::drawPhysicsConfig(fy::World *world) {
    ImGui::Begin("Physics config");

    ImGui::SliderInt("Iterations", &world->iterations, 1, 32);

    static int steps = 60;
    ImGui::SliderInt("Time step", &steps, 1, 120);
    world->timeStep = 1.0f / (float) steps;

    ImGui::InputFloat2("Gravity", &world->gravity.x);

    ImGui::End();
}

void Window::drawObjectConfig(fy::World *world) {
    ImGui::Begin("Objects");

    for (int i = 0; i < world->bodies.size(); ++i) {
        fy::Body *body = world->bodies[i];

        ImGui::PushID(i);
        if (ImGui::TreeNode(std::to_string(i).c_str())) {
            ImGui::InputFloat2("Position", &body->position.x);
            ImGui::SliderAngle("Rotation", &body->rotation, ImGuiSliderFlags_AlwaysClamp);
            ImGui::InputFloat2("Velocity", &body->velocity.x);
            ImGui::InputFloat("Angular velocity", &body->angularVelocity);

            ImGui::Spacing();

            ImGui::InputFloat("Mass", &body->mass);
            ImGui::InputFloat("Inertia", &body->inertia);
            ImGui::InputFloat("Friction", &body->friction);

            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    ImGui::End();
}
