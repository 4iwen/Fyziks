#include "Fyziks/Math/Body.h"

namespace fy {
    Body::Body() {
        position = Vec2f();
        rotation = 0.0f;
        velocity = Vec2f();
        angularVelocity = 0.0f;

        isStatic = false;
        mass = 1.0f;
        restitution = 0.5f;
        staticFriction = 0.6f;
        dynamicFriction = 0.4f;

        force = Vec2f();
        torque = 0.0f;
    }

    void Body::step(float deltaTime, Vec2f gravity) {
        if (isStatic) {
            return;
        }

        // apply gravity
        force += gravity * mass;

        // apply force and torque
        velocity += force * (deltaTime * this->getInverseMass());
        angularVelocity += torque * (deltaTime * this->getInverseInertia());

        // apply velocity
        position += velocity * deltaTime;
        rotation += angularVelocity * deltaTime;

        // reset force and torque
        force = Vec2f();
        torque = 0.0f;
    }
}