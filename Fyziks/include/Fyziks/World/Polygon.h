#pragma once

#include "Fyziks/Api.h"
#include "Body.h"

#include <vector>

namespace fy {
    class FYZIKS_API Polygon : public Body {
    public:
        std::vector<fy::Vec2> vertices;

        Polygon(std::vector<fy::Vec2> vertices, fy::Vec2 position, float rotation);
    };
}