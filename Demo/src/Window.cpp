#include "Window.h"

#include <cstdio>

Window::Window(std::string title, sf::Vector2u size) {
    this->pWindow = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(size.x, size.y),
            title,
            sf::Style::Default,
            sf::ContextSettings(0, 0, 8)
    );

    if (!ImGui::SFML::Init(*pWindow)) {
        printf("Initializing ImGui with SFML failed!\n");
        return;
    }

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Window::clear(sf::Color color) {
    pWindow->clear(color);
}

void Window::render() {
    ImGui::SFML::Render(*pWindow);
    pWindow->display();
}

Window::~Window() {
    ImGui::SFML::Shutdown();
}

bool Window::isOpen() {
    return pWindow->isOpen();
}

void Window::handleEvents() {
    sf::Event event{};

    while (pWindow->pollEvent(event)) {
        ImGui::SFML::ProcessEvent(*pWindow, event);

        if (event.type == sf::Event::Closed)
            pWindow->close();
    }
}

void Window::drawUI() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::Begin("Metrics");
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::Text("Frame time: %.2f ms", 1000.0f / ImGui::GetIO().Framerate);
    static float values[90] = {0};
    static int values_offset = 0;
    values[values_offset] = 1000.0f / ImGui::GetIO().Framerate;
    values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
    ImGui::PlotLines("Frame\ntimes", values, IM_ARRAYSIZE(values), values_offset, nullptr, 0.0f, 100.0f,
                     ImVec2(0, 80));
    ImGui::End();

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

    ImGui::Begin("Physics config");
    ImGui::Text("Physics config");
    ImGui::End();
}

void Window::drawShapeManipulator() {
    ImGui::Begin("Shape manipulator");
    if (ImGui::TreeNode("Point")) {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &pointOrigin.x, 0, 1440);
        ImGui::SliderFloat("Y", &pointOrigin.y, 0, 720);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Line")) {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &lineOrigin.x, 0, 1440);
        ImGui::SliderFloat("Y", &lineOrigin.y, 0, 720);
        ImGui::SliderFloat("Length", &lineLength, 0, 500);
        ImGui::SliderFloat("Angle", &lineAngle, 0, 360);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Triangle")) {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &triangleOrigin.x, 0, 1440);
        ImGui::SliderFloat("Y", &triangleOrigin.y, 0, 720);
        if (ImGui::TreeNode("Point 1")) {
            ImGui::Text("Position");
            ImGui::SliderFloat("X", &trianglePoint1.x, 0, 1440);
            ImGui::SliderFloat("Y", &trianglePoint1.y, 0, 720);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Point 2")) {
            ImGui::Text("Position");
            ImGui::SliderFloat("X", &trianglePoint2.x, 0, 1440);
            ImGui::SliderFloat("Y", &trianglePoint2.y, 0, 720);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Point 3")) {
            ImGui::Text("Position");
            ImGui::SliderFloat("X", &trianglePoint3.x, 0, 1440);
            ImGui::SliderFloat("Y", &trianglePoint3.y, 0, 720);
            ImGui::TreePop();
        }
        ImGui::SliderFloat("Angle", &triangleAngle, 0, 360);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Rectangle")) {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &rectangleOrigin.x, 0, 1440);
        ImGui::SliderFloat("Y", &rectangleOrigin.y, 0, 720);
        ImGui::Text("Size");
        ImGui::SliderFloat("Width", &rectangleSize.x, 0, 500);
        ImGui::SliderFloat("Height", &rectangleSize.y, 0, 500);
        ImGui::SliderFloat("Angle", &rectangleAngle, 0, 360);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Polygon")) {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &polygonOrigin.x, 0, 1440);
        ImGui::SliderFloat("Y", &polygonOrigin.y, 0, 720);
        ImGui::SliderFloat("Angle", &polygonAngle, 0, 360);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Circle")) {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &circleOrigin.x, 0, 1440);
        ImGui::SliderFloat("Y", &circleOrigin.y, 0, 720);
        ImGui::SliderFloat("Radius", &circleRadius, 0, 100);
        ImGui::SliderFloat("Angle", &circleAngle, 0, 360);
        ImGui::TreePop();
    }
    ImGui::End();
}

void Window::drawPoint(sf::Vector2f origin, sf::Color color) {
    sf::CircleShape circle;
    circle.setPosition(origin);
    circle.setRadius(2.0f);
    circle.setFillColor(color);

    pWindow->draw(circle);
}

void Window::drawLine(sf::Vector2f origin, float length, float angle, sf::Color color) {
    sf::RectangleShape line;
    line.setPosition(origin);
    line.setSize(sf::Vector2f(length, thickness));
    line.setRotation(angle);
    line.setOrigin(length / 2.0f, thickness / 2.0f);
    color.a = fillAlpha;
    line.setFillColor(color);
    color.a = outlineAlpha;
    line.setOutlineColor(color);
    line.setOutlineThickness(thickness);

    pWindow->draw(line);
}

void Window::drawTriangle(sf::Vector2f origin,
                          sf::Vector2f point1,
                          sf::Vector2f point2,
                          sf::Vector2f point3,
                          float angle,
                          sf::Color color) {
    drawPolygon(origin, {point1, point2, point3}, angle, color);
}

void Window::drawRectangle(sf::Vector2f origin, sf::Vector2f size, float angle, sf::Color color) {
    sf::RectangleShape rectangle;
    rectangle.setPosition(origin);
    rectangle.setSize(size);
    rectangle.setRotation(angle);
    rectangle.setOrigin(size / 2.0f);
    color.a = fillAlpha;
    rectangle.setFillColor(color);
    color.a = outlineAlpha;
    rectangle.setOutlineColor(color);
    rectangle.setOutlineThickness(thickness);

    pWindow->draw(rectangle);
}

void Window::drawPolygon(sf::Vector2f origin, std::vector<sf::Vector2f> vertices, float angle, sf::Color color) {
    sf::ConvexShape polygon;
    polygon.setPointCount(vertices.size());

    sf::Vector2f centroid(0.f, 0.f);
    for (const auto& vertex : vertices) {
        centroid += vertex;
    }
    centroid /= static_cast<float>(vertices.size());

    polygon.setPosition(centroid + origin);

    for (int i = 0; i < vertices.size(); i++) {
        polygon.setPoint(i, vertices[i]);
    }

    polygon.setRotation(angle);
    color.a = fillAlpha;
    polygon.setFillColor(color);
    color.a = outlineAlpha;
    polygon.setOutlineColor(color);
    polygon.setOutlineThickness(thickness);

    pWindow->draw(polygon);
}

void Window::drawCircle(sf::Vector2f origin, float radius, float angle, sf::Color color) {
    sf::CircleShape circle;
    circle.setPosition(origin);
    circle.setRadius(radius);
    circle.setOrigin(radius, radius);
    circle.setRotation(angle);
    color.a = fillAlpha;
    circle.setFillColor(color);
    color.a = outlineAlpha;
    circle.setOutlineColor(color);
    circle.setOutlineThickness(thickness);

    pWindow->draw(circle);

    drawLine(origin, radius, angle, color);
}
