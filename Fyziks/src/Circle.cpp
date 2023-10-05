#include "Fyziks/World/Circle.h"

namespace fy {
    Circle::Circle(float radius, fy::Vec2 position, float rotation) : radius(radius), Body(position, rotation) {}
}