#include "Fyziks/Physics/Body.h"

namespace fy {
    Body::Body() {
        position = Vec2f(0.0f, 0.0f);
        rotation = 0.0f;
        velocity = Vec2f(0.0f, 0.0f);
        angularVelocity = 0.0f;

        mass = 0;
        inertia = 0;
        friction = 0.2f;

        force = Vec2f(0.0f, 0.0f);
        torque = 0.0f;
    }
}