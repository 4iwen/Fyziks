#pragma once

#include "Fyziks/Api.h"
#include "Polygon.h"

namespace fy {
    class FYZIKS_API Rectangle : public Polygon {
    public:
        Rectangle(float width, float height) : width(width), height(height),
                                               Polygon(calculateVertices(width, height)) {}

        void setWidth(float w) {
            this->width = w;
            this->setVertices(calculateVertices(width, height));
        }

        void setHeight(float h) {
            this->height = h;
            this->setVertices(calculateVertices(width, height));
        }

        float getWidth() {
            return width;
        };

        float getHeight() {
            return height;
        }

    private:
        float width, height;

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