#include <cmath>
#include "App.h"

void App::run() {
    std::unique_ptr<Window> window = std::make_unique<Window>("Fyziks Demo", sf::Vector2u(1440, 720));
    sf::Clock deltaClock;

    while (window->isOpen()) {
        // handle events
        window->handleEvents();

        window->clear(sf::Color(33, 33, 33));

        // *-*-*-*-*-*-* draw -*-*-*-*-*-*-*
        ImGui::SFML::Update(*window->pWindow, deltaClock.restart());

        window->drawPoint(window->pointOrigin, window->shapeColor);
        window->drawLine(window->lineOrigin, window->lineLength, window->lineAngle, window->shapeColor);
        window->drawTriangle(window->triangleOrigin, window->trianglePoint1, window->trianglePoint2,
                             window->trianglePoint3, window->triangleAngle, window->shapeColor);
        window->drawRectangle(window->rectangleOrigin, window->rectangleSize, window->rectangleAngle,
                              window->shapeColor);
        window->drawPolygon(window->polygonOrigin,
                            {
            sf::Vector2f(-50, 100),
            sf::Vector2f(50, 100),
            sf::Vector2f(100, 50),
            sf::Vector2f(100, -50),
            sf::Vector2f(50, -100),
            sf::Vector2f(-50, -100),
            /*
                                    sf::Vector2f(1 * 100, 0 * 100),
                                    sf::Vector2f(sqrt(2) / 2 * 100, sqrt(2) / 2 * 100),
                                    sf::Vector2f(0 * 100, 1 * 100),
                                    sf::Vector2f(-sqrt(2) / 2 * 100, sqrt(2) / 2 * 100),
                                    sf::Vector2f(-1 * 100, 0 * 100),
                                    sf::Vector2f(-sqrt(2) / 2 * 100, -sqrt(2) / 2 * 100),
                                    sf::Vector2f(0 * 100, -1 * 100),,
                                    sf::Vector2f(sqrt(2) / 2 * 100, -sqrt(2) / 2 * 100),*/
                            },
                            window->polygonAngle, window->shapeColor);
        window->drawCircle(window->circleOrigin, window->circleRadius, window->circleAngle, window->shapeColor);

        window->drawUI();
        window->drawShapeManipulator();
        //ImGui::ShowDemoWindow();
        // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

        // render
        window->render();
    }
}