#pragma once

#include "Fyziks/Api.h"
#include "Body.h"

namespace fy {
    class FYZIKS_API Triangle : public Body {
    public:
        fy::Vec2 point1, point2, point3;

        Triangle(fy::Vec2 point1, fy::Vec2 point2, fy::Vec2 point3) : point1(point1), point2(point2), point3(point3),
                                                                      Body() {}
    };
}