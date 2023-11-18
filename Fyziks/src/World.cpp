#include "Fyziks/Physics/World.h"
#include "Fyziks/Physics/Collision.h"

namespace fy {
    void World::step(float deltaTime) {
        for (int i = 0; i < iterations; ++i) {
            // check for collisions
            broadPhase();

            // apply forces
            applyForces(deltaTime);

            // move objects
            moveObjects(deltaTime);
        }
    }

    void World::broadPhase() {
        for (int i = 0; i < bodies.size(); ++i) {
            bodies[i]->colliding = false;
        }

        for (int i = 0; i < bodies.size(); ++i) {
            for (int j = i + 1; j < bodies.size(); ++j) {
                if (bodies[i] == bodies[j])
                    continue;

                bool intersecting = Collision::intersects(bodies[i], bodies[j]);

                if (intersecting) {
                    bodies[i]->colliding = true;
                    bodies[j]->colliding = true;
                }
            }
        }
    }

    void World::applyForces(float deltaTime) {
        for (int i = 0; i < (int) bodies.size(); ++i) {
            auto body = bodies[i];

            if (body->getInverseMass() == 0.0f)
                continue;

            body->velocity += deltaTime * (gravity + body->getInverseMass() * body->force);
            body->angularVelocity += deltaTime * body->getInverseInertia() * body->torque;
        }
    }

    void World::moveObjects(float deltaTime) {
        for (int i = 0; i < (int) bodies.size(); ++i) {
            auto body = bodies[i];

            body->position += deltaTime * body->velocity;
            body->rotation += deltaTime * body->angularVelocity;

            body->force = Vec2f(0.0f, 0.0f);
            body->torque = 0.0f;
        }
    }
}