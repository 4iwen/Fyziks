#include "Renderer.h"
#include "../font/ProggyClean.h"
#include "imgui_internal.h"
#include "App.h"

Renderer::Renderer(sf::RenderWindow *windowInstance) {
    renderWindow = windowInstance;
    font.loadFromMemory(ProggyClean_ttf_char, ProggyClean_ttf_size);
    font.setSmooth(false);
}

void Renderer::drawWorld(World *world) {
    for (int i = 0; i < world->bodies.size(); ++i) {
        auto body = world->bodies[i];

        if (drawIDs) {
            drawID(body->position, i);
        }
        if (drawBodyNormals) {
            drawNormals(body);
        }
        if (drawAABBs) {
            drawAABB(body);
        }
        if (drawBodies) {
            drawBody(body);
        }
    }

    if (drawContactPoints) {
        for (int i = 0; i < world->contactPoints.size(); ++i) {
            auto contactPoint = world->contactPoints[i];

            drawContactPoint(contactPoint);
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
                circle->position +
                rotation * Vec2f(circle->radius * cosf(angle), circle->radius * sinf(angle));
        vertices[i] = sf::Vertex(sf::Vector2f(point.x, point.y), COLOR_WHITE);
    }
    vertices[segments + 1] = sf::Vertex(sf::Vector2f(circle->position.x, circle->position.y), COLOR_WHITE);

    // draw the circle
    renderWindow->draw(vertices, segments + 2, sf::LineStrip);
}

void Renderer::drawPolygon(Polygon *polygon) const {
    // get transformed vertices of polygon
    std::vector<Vec2f> transformedVertices = polygon->getTransformedVertices();
    // get vertices count
    unsigned long verticesCount = transformedVertices.size();
    // create an array vertices + 1
    auto *vertices = new sf::Vertex[verticesCount + 1];

    // go through the transformed vertices and add them into the array of vertices for drawing
    for (int i = 0; i < verticesCount; i++) {
        Vec2f point = transformedVertices[i];
        vertices[i] = sf::Vertex(sf::Vector2f(point.x, point.y), COLOR_WHITE);
    }
    vertices[verticesCount] = vertices[0];

    // draw the vertices
    renderWindow->draw(vertices, verticesCount + 1, sf::LineStrip);

    if (drawTriangulation) {
        std::vector<std::vector<Vec2f>> triangles = polygon->getTransformedTriangles();
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

void Renderer::drawNormals(Body *body) {
    auto polygon = body->tryCast<Polygon>();
    if (!polygon) {
        return;
    }

    if (drawTriangulation) {
        auto triangles = polygon->getTransformedTriangles();
        for (int i = 0; i < triangles.size(); ++i) {
            auto vertices = triangles[i];

            for (int j = 0; j < vertices.size(); ++j) {
                Vec2f current = vertices[j % vertices.size()];
                Vec2f next = vertices[(j + 1) % vertices.size()];
                drawNormal(current, next);
            }
        }
    } else {
        auto vertices = polygon->getTransformedVertices();
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

void Renderer::drawID(Vec2f pos, int id) const {
    sf::Text text;

    text.setFont(font);
    text.setString(std::to_string(id));
    text.setCharacterSize(16);
    text.setFillColor(COLOR_CYAN);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(pos.x, pos.y);

    renderWindow->draw(text);
}

void Renderer::drawUI(World *world, BodyProperties *bodyProps, bool &paused, float &timeStep) {
    // make the whole window dock-able
    DockSpaceOverViewport(GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    // metrics
    drawMetrics(world);
    // demos
    drawDemos(world);
    // physics config
    drawPhysicsConfig(world, paused, timeStep);
    // bodies config
    drawBodyConfig(world, bodyProps);
    // debug config
    drawDebugConfig();
    // imgui demo
    // ShowDemoWindow();
}

void Renderer::drawMetrics(World *world) {
    Begin("Metrics"); // begins a new window with a name
    Text("FPS: %.2f", GetIO().Framerate); // displays the fps as a text
    Text("Frame time: %.2f ms", 1000.0f / GetIO().Framerate); // displays the frame times as text
    static float values[100] = {0};
    static int valuesOffset = 0;
    values[valuesOffset] = 1000.0f / GetIO().Framerate;
    valuesOffset = (valuesOffset + 1) % IM_ARRAYSIZE(values);
    PlotLines("Frame\ntimes", values, IM_ARRAYSIZE(values), valuesOffset,
              nullptr, 0.0f, 240.0f,
              ImVec2(0, 80)); // displays the frame times as a chart
    Text("%zu bodies", world->bodies.size());
    Text("%zu contacts", world->contactPoints.size());

    End(); // ends the window
}

void Renderer::drawDemos(World *world) {
    Begin("Examples");
    if (TreeNode("Basic")) {
        if (BeginTable("table1", 2)) {
            const char *names[] = {
                    "Simple ground",
                    "Big box",
                    "Friction",
                    "No gravity",
                    "Restitution",
            };

            for (int i = 0; i < IM_ARRAYSIZE(names); ++i) {
                PushID(i);
                TableNextRow();
                TableSetColumnIndex(0);
                Text("%s", names[i]);
                TableSetColumnIndex(1);
                if (SmallButton("Load")) {
                    App::loadDemo(i, world);
                }
                PopID();
            }

            EndTable();
        }

        TreePop();
    }
    End();
}

void Renderer::drawPhysicsConfig(World *world, bool &paused, float &timeStep) {
    Begin("Physics config");

    Checkbox("Simulation paused", &paused);
    if (paused) {
        SameLine();
        if (Button("Step")) {
            world->step(timeStep);
        }
    }

    SliderInt("Iterations", &world->iterations, 1, 64);

    static int steps = 60;
    SliderInt("Time step", &steps, 1, 120);
    timeStep = 1.0f / (float) steps;

    DragFloat2("Gravity", &world->gravity.x);

    End();
}

void Renderer::drawBodyConfig(World *world, BodyProperties *bodyProps) {
    Begin("Bodies");

    Text("Initial properties");

    SliderAngle("Rotation", &bodyProps->rotation, 0.0f, 360.0f);
    DragFloat2("Velocity", &bodyProps->velocity.x, 0.01f);
    DragFloat("Angular velocity", &bodyProps->angularVelocity, 0.01f);

    Spacing();

    Checkbox("Is static", &bodyProps->isStatic);
    DragFloat("Mass", &bodyProps->mass, 0.01f, 0.01f, 1000);
    DragFloat("Static friction", &bodyProps->staticFriction, 0.01f, 0, 1);
    DragFloat("Dynamic friction", &bodyProps->dynamicFriction, 0.01f, 0, 1);
    DragFloat("Restitution", &bodyProps->restitution, 0.01f, 0, 1);

    Spacing();

    static const char *bodyTypes[] = {
            "Circle",
            "Triangle",
            "Rectangle",
            "Polygon",
    };

    Combo("Body type", (int *) &bodyProps->bodyType, bodyTypes, IM_ARRAYSIZE(bodyTypes));

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

            Checkbox("Is static", &body->isStatic);
            DragFloat("Mass", &body->mass, 0.01f, 0.01f, 1000);
            DragFloat("Inertia", &body->inertia, 1.0f, 1.0f, 999999.0f);
            DragFloat("Static friction", &body->staticFriction, 0.01f, 0, 1);
            DragFloat("Dynamic friction", &body->dynamicFriction, 0.01f, 0, 1);
            DragFloat("Restitution", &body->restitution, 0.01f, 0, 1);

            if (Button("Remove")) {
                world->remove(world->bodies[i]);
            }

            TreePop();
        }
        PopID();
    }
    EndChild();

    End();
}

void Renderer::drawDebugConfig() {
    Begin("Debug");

    Checkbox("Draw bodies", &drawBodies);
    Checkbox("Draw IDs", &drawIDs);
    Checkbox("Draw triangulation", &drawTriangulation);
    Checkbox("Draw normals", &drawBodyNormals);
    Checkbox("Draw AABBs", &drawAABBs);
    Checkbox("Draw contact points", &drawContactPoints);

    End();
}

void Renderer::drawAABB(Body *body) {
    AABB aabb = body->getAABB();

    Vec2f bottomLeft = Vec2f(aabb.min.x, aabb.min.y);
    Vec2f bottomRight = Vec2f(aabb.max.x, aabb.min.y);
    Vec2f topRight = Vec2f(aabb.max.x, aabb.max.y);
    Vec2f topLeft = Vec2f(aabb.min.x, aabb.max.y);

    const int vertexCount = 5;
    sf::Vertex vertices[vertexCount];
    // bottom left
    vertices[0] = sf::Vertex(sf::Vector2f(bottomLeft.x, bottomLeft.y), COLOR_TRANSPARENT_GRAY);
    // bottom right
    vertices[1] = sf::Vertex(sf::Vector2f(bottomRight.x, bottomRight.y), COLOR_TRANSPARENT_GRAY);
    // top right
    vertices[2] = sf::Vertex(sf::Vector2f(topRight.x, topRight.y), COLOR_TRANSPARENT_GRAY);
    // top left
    vertices[3] = sf::Vertex(sf::Vector2f(topLeft.x, topLeft.y), COLOR_TRANSPARENT_GRAY);
    // join the vertices
    vertices[4] = vertices[0];

    // draw the vertices
    renderWindow->draw(vertices, vertexCount, sf::LineStrip);
}

void Renderer::drawContactPoint(CollisionManifold &contact) {
    float pointSize = 4;

    sf::Vector2f pos1(contact.contactPoint1.x - pointSize / 2, contact.contactPoint1.y - pointSize / 2);

    sf::RectangleShape point1;
    point1.setSize(sf::Vector2f(pointSize, pointSize));
    point1.setPosition(pos1);
    point1.setFillColor(COLOR_TRANSPARENT_RED);

    renderWindow->draw(point1);

    if (contact.contactCount > 1) {
        sf::Vector2f pos2(contact.contactPoint2.x - pointSize / 2, contact.contactPoint2.y - pointSize / 2);


        sf::RectangleShape point2;
        point2.setSize(sf::Vector2f(pointSize, pointSize));
        point2.setPosition(pos2);
        point2.setFillColor(COLOR_TRANSPARENT_RED);

        renderWindow->draw(point2);
    }

}
