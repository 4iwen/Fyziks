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
    const std::array<sf::Color, 9> colors = {
            sf::Color(251, 73, 52),   // [0] red
            sf::Color(184, 187, 38),  // [1] light green
            sf::Color(250, 189, 47),  // [2] yellow
            sf::Color(131, 165, 152), // [3] blue
            sf::Color(211, 134, 155), // [4] purple
            sf::Color(142, 192, 124), // [5] dark green
            sf::Color(235, 219, 178), // [6] white
            sf::Color(40, 40, 40),    // [7] black
            sf::Color(146, 131, 116)  // [8] gray
    };

    Window(std::string title, sf::Vector2u size);

    ~Window();

    bool isOpen();

    void handleEvents();

    void clear(sf::Color color);

    void setVsync(bool enabled);

    void render();

    void drawUI();

    void drawRectangle(const fy::Rectangle *rectangle) const;

    void drawCircle(const fy::Circle *circle) const;

    void drawTriangle(const fy::Triangle *triangle) const;

    void drawPolygon(const fy::Polygon *polygon) const;
};
