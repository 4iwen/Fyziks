#pragma once

#include "SFML/Graphics/Color.hpp"
#include "Fyziks/Fyziks.h"
#include "BodyProperties.h"
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>

namespace fy {
    class Window {
    public:
        sf::RenderWindow *renderWindow;
        sf::View view;

        Window(const std::string &title, sf::Vector2u size);

        ~Window();

        bool isOpen() const;

        void handleEvents(World *world, BodyProperties *bodyProps);

        void updateCamera();

        void clear(sf::Color color) const;

        void render() const;

    private:
        sf::Vector2i lastMousePosition;
        bool draggingBody = false;
        Vec2f draggingBodyOffset;
        Body *pDraggingBody = nullptr;
    };
}