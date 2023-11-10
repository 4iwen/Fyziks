#include "SFML/Graphics/Color.hpp"
#include "Fyziks/Fyziks.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <imgui-SFML.h>
#include <imgui.h>
#include <memory>

#define COLOR_RED sf::Color(255, 0, 0)
#define COLOR_GREEN sf::Color(0, 255, 0)
#define COLOR_BLUE sf::Color(0, 0, 255)
#define COLOR_YELLOW sf::Color(255, 255, 0)
#define COLOR_PURPLE sf::Color(255, 0, 255)
#define COLOR_WHITE sf::Color(255, 255, 255)
#define COLOR_GRAY sf::Color(64, 64, 64)
#define COLOR_BLACK sf::Color(0, 0, 0)

class Window {
public:
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<sf::View> view;
    sf::Vector2i lastMousePosition;

    Window(std::string title, sf::Vector2u size);

    ~Window();

    bool isOpen();

    void handleEvents();

    void updateCamera();

    void clear(sf::Color color);

    void setVsync(bool enabled);

    void render();

    void drawUI(fy::World *world);

    void drawShape(const fy::Body *shape) const;

private:
    void drawMetrics();

    void drawDemos();

    void drawPhysicsConfig(fy::World *world);

    void drawObjectConfig(fy::World *world);

    void drawRectangle(const fy::Rectangle *rectangle) const;

    void drawCircle(const fy::Circle *circle) const;

    void drawTriangle(const fy::Triangle *triangle) const;

    void drawPolygon(const fy::Polygon *polygon) const;
};
