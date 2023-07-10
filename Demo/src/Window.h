#include <memory>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

class Window {
public:
    std::unique_ptr<sf::RenderWindow> pWindow;

    const sf::Color shapeColor = sf::Color(255, 0, 0);
    const float thickness = 2.0f;
    const int fillAlpha = 100;
    const int outlineAlpha = 255;

    Window(std::string title, sf::Vector2u size);

    ~Window();

    bool isOpen();

    void handleEvents();

    void clear(sf::Color color);

    void render();

    void drawUI();

    void drawShapeManipulator();

    void drawPoint(sf::Vector2f , sf::Color color = sf::Color(255, 0, 0));

    void drawLine(sf::Vector2f origin, float length, float angle = 0.0f, sf::Color color = sf::Color(255, 0, 0));

    void drawTriangle(sf::Vector2f origin, sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f point3,
                      float angle = 0.0f, sf::Color color = sf::Color(255, 0, 0));

    void drawRectangle(sf::Vector2f origin, sf::Vector2f size, float angle = 0.0f,
                       sf::Color color = sf::Color(255, 0, 0));

    void drawPolygon(sf::Vector2f origin, std::vector<sf::Vector2f> vertices, float angle = 0.0f,
                     sf::Color color = sf::Color(255, 0, 0));

    void drawCircle(sf::Vector2f origin, float radius, float angle = 0.0f, sf::Color color = sf::Color(255, 0, 0));

    sf::Vector2f pointOrigin;

    sf::Vector2f lineOrigin;
    float lineLength;
    float lineAngle;

    sf::Vector2f triangleOrigin;
    sf::Vector2f trianglePoint1;
    sf::Vector2f trianglePoint2;
    sf::Vector2f trianglePoint3;
    float triangleAngle;

    sf::Vector2f rectangleOrigin;
    sf::Vector2f rectangleSize;
    float rectangleAngle;

    sf::Vector2f polygonOrigin;
    std::vector<sf::Vector2f> polygonVertices;
    float polygonAngle;

    sf::Vector2f circleOrigin;
    float circleRadius;
    float circleAngle;
};