#include <utility>

#include "Fyziks/World/Polygon.h"

namespace fy {
    Polygon::Polygon(std::vector<fy::Vec2> vertices, fy::Vec2 position, float rotation) : vertices(std::move(vertices)),
                                                                                          Body(position, rotation) {}
}