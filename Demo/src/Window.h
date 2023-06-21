#include <memory>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

class Window {
public:
    std::unique_ptr<sf::RenderWindow> pWindow;

    Window(std::string title, sf::Vector2u size);

    ~Window();

    bool isOpen();

    void handleEvents();

    void clear(sf::Color color);

    void render();

    void drawUI();
};