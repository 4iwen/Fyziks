#include "Renderer.h"
#include "../font/ProggyClean.h"

Renderer::Renderer(sf::RenderWindow *windowInstance) {
    renderWindow = windowInstance;
    font.loadFromMemory(ProggyClean_ttf_char, ProggyClean_ttf_size);
    font.setSmooth(false);
}

void Renderer::drawWorld(World *world) {
    for (int i = 0; i < world->bodies.size(); ++i) {
        auto body = world->bodies[i];

        drawBody(body);

        if (drawIDs) {
            drawID(body->position, i);
        }

        if (drawBodyNormals) {
            drawNormals(body);
        }
    }
}

void Renderer::drawCircle(Circle *circle) const {
    // segments of the circle
    const int segments = 18;
    sf::Vertex vertices[segments + 2];
    Mat2x2 rotation(circle->rotation);
    const float angleIncrement = 2 * PIf / static_cast<float>(segments);

    // fill the vertex array
    for (int i = 0; i < segments + 1; i++) {
        float angle = static_cast<float>(i) * angleIncrement;
        Vec2f point =
                circle->position + rotation * Vec2f(circle->radius * cosf(angle), circle->radius * sinf(angle));
        vertices[i] = sf::Vertex(sf::Vector2f(point.x, point.y), COLOR_WHITE);
    }
    vertices[segments + 1] = sf::Vertex(sf::Vector2f(circle->position.x, circle->position.y), COLOR_WHITE);

    // draw the circle
    renderWindow->draw(vertices, segments + 2, sf::LineStrip);
}

void Renderer::drawPolygon(Polygon *polygon) const {
    // get translated vertices of polygon
    std::vector<Vec2f> translatedVertices = polygon->getTranslatedVertices();
    // get vertices count
    unsigned long verticesCount = translatedVertices.size();
    // create an array vertices + 1
    auto *vertices = new sf::Vertex[verticesCount + 1];

    // go through the translated vertices and add them into the array of vertices for drawing
    for (int i = 0; i < verticesCount; i++) {
        Vec2f point = translatedVertices[i];
        vertices[i] = sf::Vertex(sf::Vector2f(point.x, point.y), COLOR_WHITE);
    }
    vertices[verticesCount] = vertices[0];

    // draw the vertices
    renderWindow->draw(vertices, verticesCount + 1, sf::LineStrip);

    if (drawTriangulation) {
        std::vector<std::vector<Vec2f>> triangles = polygon->getTranslatedTriangles();
        for (int i = 0; i < triangles.size(); ++i) {
            std::vector<Vec2f> triangle = triangles[i];
            sf::Vertex vs[4];
            for (int j = 0; j < triangle.size(); ++j) {
                vs[j] = sf::Vertex(sf::Vector2f(triangle[j].x, triangle[j].y), COLOR_ORANGE);
            }
            vs[3] = vs[0];
            renderWindow->draw(vs, sizeof(vs) / sizeof(sf::Vertex), sf::LineStrip);
        }
    }

    // deallocate
    delete[] vertices;
}

void Renderer::drawBody(Body *body) const {
    // decide what shape we're dealing with
    auto circle = body->tryCast<Circle>();
    auto polygon = body->tryCast<Polygon>();

    if (circle) {
        drawCircle(circle);
    } else if (polygon) {
        drawPolygon(polygon);
    }
}


void Renderer::drawUI(World *world, bool &paused, float &timeStep) {
    // make the whole renderWindow dock-able
    DockSpaceOverViewport(GetMainViewport(),
                          ImGuiDockNodeFlags_PassthruCentralNode);
    // metrics renderWindow
    drawMetrics();
    // demos renderWindow
    drawDemos();
    // physics config
    drawPhysicsConfig(world, paused, timeStep);
    // bodies config
    drawBodyConfig(world);
    // debug config
    drawDebugConfig();
    // imgui demo renderWindow
    // ShowDemoWindow();
}

void Renderer::drawMetrics() {
    Begin("Metrics"); // begins a new renderWindow with a name
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
    End(); // ends the renderWindow
}

void Renderer::drawDemos() {
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
    End();
}

void Renderer::drawPhysicsConfig(World *world, bool &paused, float &timeStep) {
    Begin("Physics config");

    Checkbox("Simulation paused", &paused);

    SliderInt("Iterations", &world->iterations, 1, 32);

    static int steps = 60;
    SliderInt("Time step", &steps, 1, 120);
    timeStep = 1.0f / (float) steps;

    DragFloat2("Gravity", &world->gravity.x);

    End();
}

void Renderer::drawBodyConfig(World *world) {
    Begin("Bodies");

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
            "Convex polygon",
            "Concave polygon",
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
                body = world->create<Polygon>(std::vector{
                        Vec2f(0, -25),
                        Vec2f(-5, -5),
                        Vec2f(-25, 0),
                        Vec2f(-5, 5),
                        Vec2f(0, 25),
                        Vec2f(5, 5),
                        Vec2f(25, 0),
                        Vec2f(5, -5),
                });
                break;
            case 4:
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

        std::string name = "Body [" + std::to_string(i) + "]";

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

void Renderer::drawID(Vec2f pos, int id) const {
    sf::Text text;

    text.setFont(font);
    text.setString(std::to_string(id));
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(pos.x, pos.y);

    renderWindow->draw(text);
}

void Renderer::drawDebugConfig() {
    Begin("Debug");

    Checkbox("Draw IDs", &drawIDs);
    Checkbox("Draw triangulation", &drawTriangulation);
    Checkbox("Draw normals", &drawBodyNormals);

    End();
}

void Renderer::drawNormals(Body *body) {
    auto polygon = body->tryCast<Polygon>();
    if (!polygon) {
        return;
    }

    if (drawTriangulation) {
        auto triangles = polygon->getTranslatedTriangles();
        for (int i = 0; i < triangles.size(); ++i) {
            auto vertices = triangles[i];

            for (int j = 0; j < vertices.size(); ++j) {
                Vec2f current = vertices[j % vertices.size()];
                Vec2f next = vertices[(j + 1) % vertices.size()];
                drawNormal(current, next);
            }
        }
    } else {
        auto vertices = polygon->getTranslatedVertices();
        for (int j = 0; j < vertices.size(); ++j) {
            Vec2f current = vertices[j % vertices.size()];
            Vec2f next = vertices[(j + 1) % vertices.size()];
            drawNormal(current, next);
        }

    }
}

void Renderer::drawNormal(const Vec2f &current, const Vec2f &next) {
    // calculate the center of the edge
    Vec2f center = (Vec2f(current.x, current.y) + Vec2f(next.x, next.y)) * 0.5f;
    // calculate the direction vector
    Vec2f dir = Vec2f(current.x, current.y) - Vec2f(next.x, next.y);
    // get the normal of the direction vector
    Vec2f normal(dir.y, -dir.x);
    // normalize the vector
    normal.normalize();

    // specify where the second point of the normal will be
    Vec2f to = center + normal * 4.0f;

    // create the line
    sf::Vertex line[2] = {
            sf::Vertex(sf::Vector2f(center.x, center.y), COLOR_YELLOW),
            sf::Vertex(sf::Vector2f(to.x, to.y), COLOR_YELLOW)
    };

    // draw the line
    renderWindow->draw(line, sizeof(line) / sizeof(sf::Vertex), sf::LineStrip);
}
