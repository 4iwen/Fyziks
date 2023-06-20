#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <cstdio>

class App {
    std::unique_ptr<sf::RenderWindow> window;

public:
    void init();

    void run();
};