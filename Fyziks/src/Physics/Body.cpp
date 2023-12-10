#include "Fyziks/Physics/Body.h"

namespace fy {
    Body::Body() {
        position = Vec2f();
        rotation = 0.0f;
        velocity = Vec2f();
        angularVelocity = 0.0f;

        isStatic = false;

        density = 1000.0f;
        friction = 0.5f;
        restitution = 0.5f;

        force = Vec2f();
        torque = 0.0f;
    }

    void Body::step(float deltaTime, Vec2f gravity) {
        if (isStatic) {
            return;
        }

        // Newton's law of motion
        // Vec2f acceleration = force / mass;
        // velocity += acceleration * deltaTime;
        // velocity += gravity * deltaTime;
        // position += velocity * deltaTime;

        // Newton's law of motion

        // Vec2f acceleration = gravity * mass;
        // velocity += acceleration * deltaTime;
        // velocity += gravity * deltaTime;

        velocity += deltaTime * (gravity + getInverseMass() * force);
        position += velocity * deltaTime;

        // Newton's law of inertia
        rotation += angularVelocity * deltaTime;

        // body->velocity += deltaTime * (gravity + body->getInverseMass() * body->force);
        // angularVelocity += deltaTime * getInverseInertia() * torque;

        force = Vec2f();
        torque = 0.0f;
    }
}