#pragma once

#include "Fyziks/Api.h"
#include "Polygon.h"

namespace fy {
    class FYZIKS_API Triangle : public Polygon {
    public:
        Triangle(Vec2f point1, Vec2f point2, Vec2f point3) : Polygon({point1, point2, point3}) {}
    };
}