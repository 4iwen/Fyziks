#include "App.h"

#include "Fyziks.h"
#include "Fyziks/Math/Vec2.h"
#include <cstdio>

void App::run() {
  fy::Vec2 gravity;
  printf("Gravity: %f, %f\n", gravity.x, gravity.y);
  gravity.x = 0;
  gravity.y = -9.81;
  printf("Gravity: %f, %f\n", gravity.x, gravity.y);

  std::unique_ptr<Window> window =
      std::make_unique<Window>("Fyziks Demo", sf::Vector2u(1440, 720));
  sf::Clock deltaClock;

  while (window->isOpen()) {
    // handle events
    window->handleEvents();
    // clear the window
    window->clear(sf::Color(33, 33, 33));

    // *-*-*-*-*-*-* draw -*-*-*-*-*-*-*
    ImGui::SFML::Update(*window->window, deltaClock.restart());
    // draw shapes
    std::vector<fy::Vec2> polygonVertices = {
        fy::Vec2(500, 500), fy::Vec2(600, 500), fy::Vec2(600, 600),
        fy::Vec2(500, 600)};
    window->drawPolygon(polygonVertices, window->colors[3], window->colors[2]);
    window->drawLine(fy::Vec2(400, 200), fy::Vec2(500, 300), window->colors[1]);

    // draw imgui
    window->drawUI();
    // ImGui::ShowDemoWindow();
    // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

    // render
    window->render();
  }
}
