#include "Fyziks/Physics/Body.h"

namespace fy {
    Body::Body() {
        position = Vec2(0.0f, 0.0f);
        rotation = 0.0f;
        velocity = Vec2(0.0f, 0.0f);
        angularVelocity = 0.0f;

        mass = std::numeric_limits<float>::max();
        inertia = std::numeric_limits<float>::max();
        friction = 0.2f;

        force = Vec2(0.0f, 0.0f);
        torque = 0.0f;
    }
}