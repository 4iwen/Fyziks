#pragma once

#include "Fyziks/Api.h"
#include "Body.h"

namespace fy {
    class FYZIKS_API Triangle : public Body {
    public:
        fy::Vec2 point1, point2, point3;

        Triangle(fy::Vec2 point1, fy::Vec2 point2, fy::Vec2 point3, fy::Vec2 position, float rotation);
    };
}