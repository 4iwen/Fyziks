#include "Window.h"

#include <cstdio>
#include <imgui.h>

using namespace fy;
using namespace ImGui;

Window::Window(const std::string &title, const sf::Vector2u size) {
    // create window
    this->renderWindow = new sf::RenderWindow(
            sf::VideoMode(size.x, size.y), title, sf::Style::Default,
            sf::ContextSettings(0, 0, 8));

    this->view = sf::View(sf::FloatRect(0, 0, 0, 0));
    renderWindow->setView(view);
    lastMousePosition = sf::Mouse::getPosition();

    // initialize SFML
    if (!SFML::Init(*renderWindow)) {
        printf("Initializing ImGui with SFML failed!\n");
        return;
    }

    // enable docking for imgui
    GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Window::clear(sf::Color color) const { renderWindow->clear(color); }

void Window::render() const {
    SFML::Render(*renderWindow);
    renderWindow->display();
}

Window::~Window() {
    SFML::Shutdown();
    delete renderWindow;
}

bool Window::isOpen() const { return renderWindow->isOpen(); }

void Window::handleEvents() {
    sf::Event event{};

    while (renderWindow->pollEvent(event)) {
        SFML::ProcessEvent(*renderWindow, event);

        if (event.type == sf::Event::Closed) {
            renderWindow->close();
        }
        if (event.type == sf::Event::Resized) {
            view.setSize((float) event.size.width, (float) event.size.height);
            renderWindow->setView(view);
        }
        if (event.type == sf::Event::MouseWheelScrolled) {
            if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
                view.zoom(1.0f + -event.mouseWheelScroll.delta * 0.1f);
                renderWindow->setView(view);
            }
        }
    }
}

void Window::updateCamera() {
    // get the current mouse position
    sf::Vector2i currentMousePosition = sf::Mouse::getPosition();
    // check if the left mouse button is pressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        // calculate the offset between the current and last mouse positions
        sf::Vector2f offset =
                renderWindow->mapPixelToCoords(currentMousePosition) -
                renderWindow->mapPixelToCoords(lastMousePosition);
        // move the view / camera
        view.move(-offset);
        renderWindow->setView(view);
    }
    // update the last mouse position for the next frame
    lastMousePosition = currentMousePosition;
}