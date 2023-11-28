#pragma once

#include <SFML/Graphics.hpp>
#include <imgui.h>

#include "Fyziks/Fyziks.h"

using namespace fy;
using namespace ImGui;

#define COLOR_RED       sf::Color(255,   0,   0)
#define COLOR_GREEN     sf::Color(  0, 255,   0)
#define COLOR_BLUE      sf::Color(  0,   0, 255)
#define COLOR_YELLOW    sf::Color(255, 255,   0)
#define COLOR_ORANGE    sf::Color(255, 128,   0)
#define COLOR_PURPLE    sf::Color(255,   0, 255)
#define COLOR_WHITE     sf::Color(255, 255, 255)
#define COLOR_GRAY      sf::Color( 64,  64,  64)
#define COLOR_BLACK     sf::Color(  0,   0,   0)

class Renderer {
public:
    Renderer(sf::RenderWindow *windowInstance);

    // imgui related
    void drawUI(World *world, bool &paused, float &timeStep);

    // fyziks related
    void drawWorld(World *world);

private:
    sf::RenderWindow *renderWindow;
    sf::Font font;

    bool drawIDs = true;
    bool drawTriangulation = false;
    bool drawBodyNormals = false;

    // imgui related
    void drawMetrics();

    void drawDemos();

    void drawDebugConfig();

    void drawPhysicsConfig(World *world, bool &paused, float &timeStep);

    void drawBodyConfig(World *world);

    // fyziks related
    void drawBody(Body *body) const;

    void drawCircle(Circle *circle) const;

    void drawPolygon(Polygon *polygon) const;

    void drawID(Vec2f pos, int id) const;

    void drawNormals(Body *body);

    void drawNormal(const Vec2f &current, const Vec2f &next);
};