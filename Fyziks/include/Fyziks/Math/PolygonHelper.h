#pragma once

#include <vector>
#include <array>
#include "Fyziks/Math/Vec2f.h"

namespace fy {
    class FYZIKS_API PolygonHelper {
    public:
        static std::vector<std::vector<Vec2f>> triangulate(std::vector<Vec2f> vertices);

        static bool
        isPointInsideTriangle(const Vec2f &point, const Vec2f &prev, const Vec2f &current, const Vec2f &next);

    private:

        static bool isClockwise(const std::vector<Vec2f> &vertices);

        static bool isConvex(const Vec2f &prev, const Vec2f &current, const Vec2f &next);

        static bool isEar(const std::vector<Vec2f> &vertices,
                          unsigned int prevIndex, unsigned int currentIndex, unsigned int nextIndex);
    };
}