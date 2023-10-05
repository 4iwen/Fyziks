#include "Fyziks/World/Triangle.h"

namespace fy {
    Triangle::Triangle(fy::Vec2 point1, fy::Vec2 point2, fy::Vec2 point3, fy::Vec2 position, float rotation) : point1(
            point1), point2(point2), point3(point3), Body(position, rotation) {}
}