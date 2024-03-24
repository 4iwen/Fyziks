#pragma once

#include "Fyziks/Api.h"
#include "Fyziks/Math/Vec2f.h"

namespace fy {
    class FYZIKS_API AABB {
    public:
        Vec2f min;
        Vec2f max;

        AABB() : min(0, 0), max(0, 0) {}

        AABB(float minX, float minY, float maxX, float maxY) : min(minX, minY), max(maxX, maxY) {}

        AABB(Vec2f min, Vec2f max) : min(min), max(max) {}
    };
}