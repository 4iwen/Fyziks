#include <Fyziks/World/Rectangle.h>

namespace fy {
    Rectangle::Rectangle(float width, float height, fy::Vec2 position, float rotation) : width(width), height(height),
                                                                                         Body(position, rotation) {}
}