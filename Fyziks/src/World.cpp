#include "Fyziks/Physics/World.h"
#include "Fyziks/Physics/Collision.h"

namespace fy {
    void World::step() {
        for (int i = 0; i < iterations; ++i) {
            // check for collisions
            broadPhase();

            // apply forces
            applyForces();

            // move objects
            moveObjects();
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

    void World::applyForces() {
        for (int i = 0; i < (int) bodies.size(); ++i) {
            Body *body = bodies[i];

            if (body->getInverseMass() == 0.0f)
                continue;

            body->velocity += timeStep * (gravity + body->getInverseMass() * body->force);
            body->angularVelocity += timeStep * body->getInverseInertia() * body->torque;
        }
    }

    void World::moveObjects() {
        for (int i = 0; i < (int) bodies.size(); ++i) {
            Body *body = bodies[i];

            body->position += timeStep * body->velocity;
            body->rotation += timeStep * body->angularVelocity;

            body->force = Vec2f(0.0f, 0.0f);
            body->torque = 0.0f;
        }
    }
}