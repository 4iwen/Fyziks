#include "SFML/Graphics/Color.hpp"
#include <Fyziks/Math/Vec2.h>
#include <SFML/Graphics.hpp>
#include <array>
#include <imgui-SFML.h>
#include <imgui.h>
#include <memory>

class Window {
public:
  std::unique_ptr<sf::RenderWindow> window;
  const std::array<sf::Color, 16> colors = {
      sf::Color(204, 36, 29),   // red fill
      sf::Color(251, 73, 52),   // red outline
      sf::Color(152, 151, 26),  // light green fill
      sf::Color(184, 187, 38),  // light green outline
      sf::Color(215, 153, 33),  // yellow fill
      sf::Color(250, 189, 47),  // yellow outline
      sf::Color(69, 133, 136),  // blue fill
      sf::Color(131, 165, 152), // blue outline
      sf::Color(177, 98, 134),  // purple fill
      sf::Color(211, 134, 155), // purple outline
      sf::Color(104, 157, 106), // dark green fill
      sf::Color(142, 192, 124), // dark green outline
      sf::Color(168, 153, 132), // white fill
      sf::Color(235, 219, 178), // white outline
      sf::Color(40, 40, 40),    // black
      sf::Color(146, 131, 116)  // gray
  };

  Window(std::string title, sf::Vector2u size);

  ~Window();

  bool isOpen();

  void handleEvents();

  void clear(sf::Color color);

  void render();

  void drawUI();

  void drawPoint(fy::Vec2 pos, sf::Color color);

  void drawLine(fy::Vec2 pos1, fy::Vec2 pos2, sf::Color color);

  void drawTriangle(fy::Vec2 pos1, fy::Vec2 pos2, fy::Vec2 pos3,
                    sf::Color outlineColor, sf::Color fillColor);

  void drawRectangle(fy::Vec2 pos, fy::Vec2 size, sf::Color outlineColor,
                     sf::Color fillColor);

  void drawPolygon(std::vector<fy::Vec2> vertices, sf::Color outlineColor,
                   sf::Color fillColor);

  void drawCircle(fy::Vec2 pos, float radius, sf::Color outlineColor,
                  sf::Color fillColor);
};
