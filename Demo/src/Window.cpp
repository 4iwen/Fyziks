#include "Window.h"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/System/Vector2.hpp"

#include <cstdio>

Window::Window(std::string title, sf::Vector2u size) {
  this->window = std::make_unique<sf::RenderWindow>(
      sf::VideoMode(size.x, size.y), title, sf::Style::Default,
      sf::ContextSettings(0, 0, 8));

  if (!ImGui::SFML::Init(*window)) {
    printf("Initializing ImGui with SFML failed!\n");
    return;
  }

  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Window::clear(sf::Color color) { window->clear(color); }

void Window::render() {
  ImGui::SFML::Render(*window);
  window->display();
}

Window::~Window() { ImGui::SFML::Shutdown(); }

bool Window::isOpen() { return window->isOpen(); }

void Window::handleEvents() {
  sf::Event event{};

  while (window->pollEvent(event)) {
    ImGui::SFML::ProcessEvent(*window, event);

    if (event.type == sf::Event::Closed)
      window->close();
  }
}

void Window::drawUI() {
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(),
                               ImGuiDockNodeFlags_PassthruCentralNode);

  ImGui::Begin("Metrics");
  ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
  ImGui::Text("Frame time: %.2f ms", 1000.0f / ImGui::GetIO().Framerate);
  static float values[90] = {0};
  static int values_offset = 0;
  values[values_offset] = 1000.0f / ImGui::GetIO().Framerate;
  values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
  ImGui::PlotLines("Frame\ntimes", values, IM_ARRAYSIZE(values), values_offset,
                   nullptr, 0.0f, 100.0f, ImVec2(0, 80));
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

void Window::drawPoint(fy::Vec2 pos, sf::Color color) {
  sf::CircleShape circle;
  circle.setPosition({pos.x, pos.y});
  circle.setFillColor(color);

  window->draw(circle);
}

void Window::drawLine(fy::Vec2 pos1, fy::Vec2 pos2, sf::Color color) {
  sf::VertexArray line(sf::TriangleStrip, 2);
  line[0].position = sf::Vector2f(pos1.x, pos1.y);
  line[1].position = sf::Vector2f(pos2.x, pos2.y);
  line[0].color = color;
  line[1].color = color;
}

void Window::drawTriangle(fy::Vec2 pos1, fy::Vec2 pos2, fy::Vec2 pos3,
                          sf::Color outlineColor, sf::Color fillColor) {
  drawPolygon({pos1, pos2, pos3}, outlineColor, fillColor);
}

void Window::drawRectangle(fy::Vec2 pos, fy::Vec2 size, sf::Color outlineColor,
                           sf::Color fillColor) {
  sf::RectangleShape rectangle;
  rectangle.setPosition({pos.x, pos.y});
  rectangle.setSize({size.x, size.y});
  rectangle.setFillColor(fillColor);
  rectangle.setOutlineColor(outlineColor);
  window->draw(rectangle);
}

void Window::drawPolygon(std::vector<fy::Vec2> vertices, sf::Color outlineColor,
                         sf::Color fillColor) {
  sf::ConvexShape polygon;
  polygon.setPointCount(vertices.size());
  for (int i = 0; i < vertices.size(); i++) {
    polygon.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));
  }
  fillColor.a = 100;
  polygon.setFillColor(fillColor);
  polygon.setOutlineColor(outlineColor);
  window->draw(polygon);
}

void Window::drawCircle(fy::Vec2 pos, float radius, sf::Color outlineColor,
                        sf::Color fillColor) {}
