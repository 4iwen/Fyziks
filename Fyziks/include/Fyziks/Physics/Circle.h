#pragma once

#include "Fyziks/Api.h"
#include "Body.h"

namespace fy {
    class FYZIKS_API Circle : public Body {
    public:
        float radius;

        Circle(float radius) : radius(radius) {
            this->mass = PIf * radius * radius * density;
            this->inertia = 0.5f * mass * radius * radius;
        }

        AABB getAABB() override {
            return {
                    position.x - radius, position.y - radius,
                    position.x + radius, position.y + radius
            };
        }
    };
}