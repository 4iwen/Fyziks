#pragma once

#include <vector>
#include <array>
#include "Fyziks/Math/Vec2f.h"

namespace fy {
    class FYZIKS_API PolygonHelper {
    public:
        static std::vector<std::vector<Vec2f>> triangulate(std::vector<Vec2f> vertices);

        static bool isPointInsideTriangle(const Vec2f &point,
                                          const Vec2f &prev, const Vec2f &current, const Vec2f &next);

        static float calculateTriangleArea(const Vec2f &point1, const Vec2f &point2, const Vec2f &point3);

        static Vec2f calculateCentroid(const std::vector<Vec2f> &vertices);

    private:

        static bool isClockwise(const std::vector<Vec2f> &vertices);

        static bool isConvex(const Vec2f &prev, const Vec2f &current, const Vec2f &next);

        static bool isEar(const std::vector<Vec2f> &vertices,
                          unsigned int prevIndex, unsigned int currentIndex, unsigned int nextIndex);
    };
}