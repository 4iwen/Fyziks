#include "Window.h"

#include <imgui.h>

using namespace fy;
using namespace ImGui;

Window::Window(const std::string &title, const sf::Vector2u size) {
    // create window
    this->renderWindow = new sf::RenderWindow(
            sf::VideoMode(size.x, size.y), title, sf::Style::Default,
            sf::ContextSettings(0, 0, 8));

    this->view = sf::View(sf::FloatRect(0, 0, (float) size.x, (float) size.y));
    view.setCenter(0, 0);
    renderWindow->setView(view);
    lastMousePosition = sf::Mouse::getPosition();

    // initialize SFML
    if (!SFML::Init(*renderWindow)) {
        throw std::runtime_error("Initializing ImGui with SFML failed!");
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

void Window::handleEvents(World *world, BodyProperties *bodyProps) {
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
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
                    Body *body;

                    switch (bodyProps->bodyType) {
                        case CIRCLE:
                            body = world->create<Circle>(25);
                            break;
                        case TRIANGLE:
                            body = world->create<Triangle>(
                                    Vec2f(-21.65, 12.5),
                                    Vec2f(21.65, 12.5),
                                    Vec2f(0, -25)
                            );
                            break;
                        case RECTANGLE:
                            body = world->create<Rectangle>(75, 50);
                            break;
                        case POLYGON:
                            body = world->create<Polygon>(std::vector{
                                    Vec2f(0, -25),
                                    Vec2f(-5, -5),
                                    Vec2f(-25, 0),
                                    Vec2f(-5, 5),
                                    Vec2f(0, 25),
                                    Vec2f(5, 5),
                                    Vec2f(25, 0),
                                    Vec2f(5, -5),
                            });
                            break;
                    }


                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow);
                    sf::Vector2f position = renderWindow->mapPixelToCoords(mousePosition);

                    body->position = Vec2f(position.x, position.y);
                    body->rotation = bodyProps->rotation;
                    body->velocity = bodyProps->velocity;
                    body->angularVelocity = bodyProps->angularVelocity;
                    body->isStatic = bodyProps->isStatic;
                    body->mass = bodyProps->mass;
                    body->restitution = bodyProps->restitution;
                    body->staticFriction = bodyProps->staticFriction;
                    body->dynamicFriction = bodyProps->dynamicFriction;
                }
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