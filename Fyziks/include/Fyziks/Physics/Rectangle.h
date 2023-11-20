#pragma once

#include "Fyziks/Api.h"
#include "Polygon.h"

namespace fy {
    class FYZIKS_API Rectangle : public Polygon {
    public:
        float width, height;

        Rectangle(float width, float height) : width(width), height(height),
                                               Polygon(calculateVertices(width, height)) {}

    private:
        std::vector<Vec2f> calculateVertices(float w, float h) {
            Vec2f half(w * 0.5f, h * 0.5f);

            // calculate the 4 vertices of the rectangle
            Vec2f v1 = Vec2f(-half.x, -half.y);
            Vec2f v2 = Vec2f(-half.x, half.y);
            Vec2f v3 = Vec2f(half.x, half.y);
            Vec2f v4 = Vec2f(half.x, -half.y);

            return {v1, v2, v3, v4};
        }
    };
}