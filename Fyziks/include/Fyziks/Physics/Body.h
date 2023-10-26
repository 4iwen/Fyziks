#pragma once

#include "Fyziks/Api.h"
#include "Fyziks/Math/Vec2.h"

#include <limits>

namespace fy {
    class FYZIKS_API Body {
    public:
        // state
        fy::Vec2 position;
        float rotation;
        fy::Vec2 velocity;
        float angularVelocity;

        // properties
        float mass;
        float inertia;
        float friction;

        // applied forces
        fy::Vec2 force;
        float torque;

        void addForce(fy::Vec2 f) {
            this->force += f;
        }

        float inverseMass() const {
            if (mass != 0.0f) {
                return 1.0f / mass;
            } else {
                return 0.0f;
            }
        }

        float inverseInertia() const {
            if (inertia != 0.0f) {
                return 1.0f / inertia;
            } else {
                return 0.0f;
            }
        }

        virtual ~Body() {}

    protected:
        Body();
    };
}
