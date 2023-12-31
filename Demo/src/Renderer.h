#pragma once

#include <SFML/Graphics.hpp>
#include <imgui.h>

#include "Fyziks/Fyziks.h"
#include "BodyProperties.h"

using namespace fy;
using namespace ImGui;

#define COLOR_RED                   sf::Color(255,   0,   0, 255)
#define COLOR_TRANSPARENT_RED       sf::Color(255,   0,   0, 128)
#define COLOR_GREEN                 sf::Color(  0, 255,   0, 255)
#define COLOR_BLUE                  sf::Color(  0,   0, 255, 255)
#define COLOR_YELLOW                sf::Color(255, 255,   0, 255)
#define COLOR_MAGENTA               sf::Color(255,   0, 255, 255)
#define COLOR_CYAN                  sf::Color(  0, 255, 255, 255)
#define COLOR_ORANGE                sf::Color(255, 128,   0, 255)
#define COLOR_WHITE                 sf::Color(255, 255, 255, 255)
#define COLOR_GRAY                  sf::Color( 64,  64,  64, 255)
#define COLOR_TRANSPARENT_GRAY      sf::Color(128, 128, 128, 128)
#define COLOR_BLACK                 sf::Color(  0,   0,   0, 255)

class Renderer {
public:
    Renderer(sf::RenderWindow *windowInstance);

    // imgui related
    void drawUI(World *world, BodyProperties *bodyProps, bool &paused, float &timeStep);

    // fyziks related
    void drawWorld(World *world);

private:
    sf::RenderWindow *renderWindow;
    sf::Font font;

    bool drawBodies = true;
    bool drawIDs = true;
    bool drawTriangulation = false;
    bool drawBodyNormals = false;
    bool drawAABBs = false;
    bool drawContactPoints = false;

    // imgui related
    void drawMetrics(World *world);

    void drawDemos(World *world);

    void drawDebugConfig();

    void drawPhysicsConfig(World *world, bool &paused, float &timeStep);

    void drawBodyConfig(World *world, BodyProperties *bodyProps);

    // fyziks related
    void drawBody(Body *body) const;

    void drawCircle(Circle *circle) const;

    void drawPolygon(Polygon *polygon) const;

    void drawID(Vec2f pos, int id) const;

    void drawNormals(Body *body);

    void drawNormal(const Vec2f &current, const Vec2f &next);

    void drawAABB(Body *body);

    void drawContactPoint(CollisionManifold &contact);
};