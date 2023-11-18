#include "Window.h"

#include <cstdio>
#include <cmath>
#include <imgui.h>

using namespace fy;
using namespace ImGui;

sf::Color defaultColor = COLOR_WHITE;
sf::Color collisionColor = COLOR_RED;

Window::Window(const std::string &title, const sf::Vector2u size) {
    this->window = new sf::RenderWindow(
            sf::VideoMode(size.x, size.y), title, sf::Style::Default,
            sf::ContextSettings(0, 0, 8));

    this->view = sf::View(sf::FloatRect(0, 0, 0, 0));
    window->setView(view);

    lastMousePosition = sf::Mouse::getPosition();

    if (!SFML::Init(*window)) {
        printf("Initializing ImGui with SFML failed!\n");
        return;
    }

    GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Window::clear(sf::Color color) const { window->clear(color); }

void Window::render() const {
    SFML::Render(*window);
    window->display();
}

Window::~Window() {
    SFML::Shutdown();
    delete this->window;
}

bool Window::isOpen() const { return window->isOpen(); }

void Window::handleEvents() {
    sf::Event event{};

    while (window->pollEvent(event)) {
        SFML::ProcessEvent(*window, event);

        if (event.type == sf::Event::Closed) {
            window->close();
        }
        if (event.type == sf::Event::Resized) {
            sf::Vector2f viewCenter = view.getCenter();
            view.setSize((float) event.size.width, (float) event.size.height);
            view.setCenter(viewCenter);
            window->setView(view);
        }
        if (event.type == sf::Event::MouseWheelScrolled) {
            view.zoom(1.0f + -event.mouseWheelScroll.delta * 0.1f);
            window->setView(view);
        }
    }
}

void Window::updateCamera() {
    // get the current mouse position
    sf::Vector2i currentMousePosition = sf::Mouse::getPosition();
    // check if the left mouse button is pressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        // calculate the offset between the current and last mouse positions
        sf::Vector2f offset =
                window->mapPixelToCoords(currentMousePosition) - window->mapPixelToCoords(lastMousePosition);
        // move the view / camera
        view.move(-offset);
        window->setView(view);
    }
    // update the last mouse position for the next frame
    lastMousePosition = currentMousePosition;
}

void Window::setVsync(bool enabled) const {
    window->setVerticalSyncEnabled(enabled);
}

void Window::drawUI(World *world, bool &paused, float &timeStep) {
    // make the whole window dock-able
    DockSpaceOverViewport(GetMainViewport(),
                          ImGuiDockNodeFlags_PassthruCentralNode);
    // metrics window
    drawMetrics();
    // demos window
    drawDemos();
    // physics config
    drawPhysicsConfig(world, paused, timeStep);
    // objects
    drawObjectConfig(world);
    // imgui demo window
    // ShowDemoWindow();
}

void Window::drawRectangle(Rectangle *rectangle) const {
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

    // add to the vertex array
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

void Window::drawCircle(Circle *circle) const {
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

void Window::drawTriangle(Triangle *triangle) const {
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

void Window::drawPolygon(Polygon *polygon) const {
    // get translated vertices of polygon
    std::vector<Vec2f> translatedVertices = polygon->getTranslatedVertices();
    // get vertices count
    unsigned long verticesCount = translatedVertices.size();
    // create an array vertices + 1
    auto *vertices = new sf::Vertex[verticesCount + 1];

    sf::Color color;
    if (polygon->colliding) {
        color = collisionColor;
    } else {
        color = defaultColor;
    }

    // go through the translated vertices and add them into the array of vertices for drawing
    for (int i = 0; i < verticesCount; i++) {
        Vec2f point = translatedVertices[i];
        vertices[i] = sf::Vertex(sf::Vector2f(point.x, point.y), color);
    }
    vertices[verticesCount] = vertices[0];

    // draw the vertices
    window->draw(vertices, verticesCount + 1, sf::LineStrip);

    // deallocate
    delete[] vertices;
}

void Window::drawBody(Body *body, int id) const {
    // decide what shape we're dealing with
    auto rectangle = body->castAndCheck<Rectangle>();
    auto circle = body->castAndCheck<Circle>();
    auto triangle = body->castAndCheck<Triangle>();
    auto polygon = body->castAndCheck<Polygon>();

    // draw it
    if (rectangle) {
        drawRectangle(rectangle);
    } else if (circle) {
        drawCircle(circle);
    } else if (triangle) {
        drawTriangle(triangle);
    } else if (polygon) {
        drawPolygon(polygon);
    }

    // draw the id of the shape
    drawID(body->position, id);
}

void Window::drawMetrics() {
    Begin("Metrics"); // begins a new window with a name
    Text("FPS: %.2f",
         GetIO().Framerate); // displays the fps as a text
    Text("Frame time: %.2f ms",
         1000.0f /
         GetIO().Framerate); // displays the frame times as text
    static float values[90] = {0};
    static int values_offset = 0;
    values[values_offset] = 1000.0f / GetIO().Framerate;
    values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
    PlotLines("Frame\ntimes", values, IM_ARRAYSIZE(values), values_offset,
              nullptr, 0.0f, 100.0f,
              ImVec2(0, 80)); // displays the frame times as a chart
    End(); // ends the window
}

void Window::drawDemos() {
    Begin("Examples");
    if (TreeNode("Basic")) {
        if (BeginTable("table1", 2)) {
            TableNextRow();
            TableSetColumnIndex(0);
            Text("Basic example");
            TableSetColumnIndex(1);
            SmallButton("Load");
            EndTable();
        }

        TreePop();
    }
    if (TreeNode("Advanced")) {
        if (BeginTable("table2", 2)) {
            TableNextRow();
            TableSetColumnIndex(0);
            Text("Advanced example");
            TableSetColumnIndex(1);
            SmallButton("Load");
            EndTable();
        }

        TreePop();
    }
    if (TreeNode("Explosions")) {
        if (BeginTable("table3", 2)) {
            TableNextRow();
            TableSetColumnIndex(0);
            Text("Explosions example");
            TableSetColumnIndex(1);
            SmallButton("Load");
            EndTable();
        }

        TreePop();
    }
    End();
}

void Window::drawPhysicsConfig(World *world, bool &paused, float &timeStep) {
    Begin("Physics config");

    Checkbox("Simulation paused", &paused);

    SliderInt("Iterations", &world->iterations, 1, 32);

    static int steps = 60;
    SliderInt("Time step", &steps, 1, 120);
    timeStep = 1.0f / (float) steps;

    DragFloat2("Gravity", &world->gravity.x);

    End();
}

void Window::drawObjectConfig(World *world) {
    Begin("Objects");

    Text("Initial properties");

    static Vec2f initialPosition = Vec2f();
    static float initialRotation = 0.0f;
    static Vec2f initialVelocity = Vec2f();
    static float initialAngularVelocity = 0.0f;

    DragFloat2("Position", &initialPosition.x);
    SliderAngle("Rotation", &initialRotation, 0.0f, 360.0f);
    DragFloat2("Velocity", &initialVelocity.x, 0.01f);
    DragFloat("Angular velocity", &initialAngularVelocity, 0.01f);

    Spacing();

    static float initialMass = 0.0f;
    static float initialInertia = 0.0f;
    static float initialFriction = 0.2f;
    DragFloat("Mass", &initialMass, 0.01f);
    DragFloat("Inertia", &initialInertia, 0.01f);
    DragFloat("Friction", &initialFriction, 0.01f);


    Spacing();

    static const char *bodyTypes[] = {
            "Triangle",
            "Rectangle",
            "Polygon",
            "Circle",
    };
    static int currentBodyTypeSelected = 0;

    Combo("Body type", &currentBodyTypeSelected, bodyTypes, IM_ARRAYSIZE(bodyTypes));

    if (Button("Add body")) {
        Body *body;

        switch (currentBodyTypeSelected) {
            default:
            case 0:
                body = world->create<Triangle>(
                        Vec2f(-21.65, 12.5),
                        Vec2f(21.65, 12.5),
                        Vec2f(0, -25)
                );
                break;
            case 1:
                body = world->create<Rectangle>(50, 50);
                break;
            case 2:
                body = world->create<Polygon>(std::vector{
                        Vec2f(0, -25),
                        Vec2f(-21.65, -12.5),
                        Vec2f(-21.65, 12.5),
                        Vec2f(0, 25),
                        Vec2f(21.65, 12.5),
                        Vec2f(21.65, -12.5),
                });
                break;
            case 3:
                body = world->create<Circle>(25);
                break;
        }

        body->position = initialPosition;
        body->rotation = initialRotation;
        body->velocity = initialVelocity;
        body->angularVelocity = initialAngularVelocity;

        body->mass = initialMass;
        body->inertia = initialInertia;
        body->friction = initialFriction;
    }

    if (Button("Remove all bodies")) {
        world->clear();
    }

    Spacing();

    Text("%zu bodies", world->bodies.size());
    BeginChild("Bodies", ImVec2(0, 0), true);
    for (int i = 0; i < world->bodies.size(); ++i) {
        auto body = world->bodies[i];

        std::string name = "Object [" + std::to_string(i) + "]";

        PushID(i);
        if (TreeNode(name.c_str())) {
            DragFloat2("Position", &body->position.x);
            SliderAngle("Rotation", &body->rotation, 0.0f, 360.0f);
            DragFloat2("Velocity", &body->velocity.x, 0.01f);
            DragFloat("Angular velocity", &body->angularVelocity, 0.01f);

            Spacing();

            DragFloat("Mass", &body->mass, 0.01f);
            DragFloat("Inertia", &body->inertia, 0.01f);
            DragFloat("Friction", &body->friction, 0.01f);

            if (Button("Delete")) {
                world->remove(world->bodies[i]);
            }

            TreePop();
        }
        PopID();
    }
    EndChild();

    End();
}

void Window::drawID(Vec2f pos, int id) const {
    ImVec2 p;

    sf::Vector2 viewPos = window->mapCoordsToPixel(sf::Vector2f(pos.x, pos.y));
    p.x = float(viewPos.x);
    p.y = float(viewPos.y);

    SetNextWindowPos(ImVec2(0, 0));
    Begin("ID", nullptr,
          ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs |
          ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar |
          ImGuiViewportFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

    std::string text_str = std::to_string(id);
    const char *text_c_str = text_str.c_str();

    ImVec2 textSize = CalcTextSize(text_c_str);
    SetCursorPos(ImVec2(p.x - textSize.x / 2, p.y - textSize.y / 2));
    Text("%s", text_c_str);
    End();
}
