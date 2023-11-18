#pragma once

#include "SFML/Graphics/Color.hpp"
#include "Fyziks/Fyziks.h"
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>

#define COLOR_RED sf::Color(255, 0, 0)
#define COLOR_GREEN sf::Color(0, 255, 0)
#define COLOR_BLUE sf::Color(0, 0, 255)
#define COLOR_YELLOW sf::Color(255, 255, 0)
#define COLOR_PURPLE sf::Color(255, 0, 255)
#define COLOR_WHITE sf::Color(255, 255, 255)
#define COLOR_GRAY sf::Color(64, 64, 64)
#define COLOR_BLACK sf::Color(0, 0, 0)

namespace fy {
    class Window {
    public:
        sf::RenderWindow *window;
        sf::View view;
        sf::Vector2i lastMousePosition;

        Window(const std::string &title, sf::Vector2u size);

        ~Window();

        bool isOpen() const;

        void handleEvents();

        void updateCamera();

        void clear(sf::Color color) const;

        void setVsync(bool enabled) const;

        void render() const;

        void drawUI(World *world, bool &paused, float &timeStep);

        void drawBody(Body *body, int id) const;

    private:
        void drawMetrics();

        void drawDemos();

        void drawPhysicsConfig(World *world, bool &paused, float &timeStep);

        void drawObjectConfig(World *world);

        void drawRectangle(Rectangle *rectangle) const;

        void drawCircle(Circle *circle) const;

        void drawTriangle(Triangle *triangle) const;

        void drawPolygon(Polygon *polygon) const;

        void drawID(Vec2f pos, int id) const;
    };
}