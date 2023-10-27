#include "SFML/Graphics/Color.hpp"
#include "Fyziks/Fyziks.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <imgui-SFML.h>
#include <imgui.h>
#include <memory>

class Window {
public:
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<sf::View> view;
    sf::Vector2i lastMousePosition;

    const std::array<sf::Color, 9> colors = {
            sf::Color(255, 0, 0),     // [0] red
            sf::Color(0, 255, 0),     // [1] green
            sf::Color(0, 0, 255),     // [2] blue
            sf::Color(255, 255, 0),   // [3] yellow
            sf::Color(255, 0, 255),   // [4] purple
            sf::Color(255, 255, 255), // [5] white
            sf::Color(64, 64, 64),    // [6] gray
            sf::Color(0, 0, 0),       // [7] black
    };

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
