#include "Fyziks/Physics/World.h"
#include "Fyziks/Physics/Collision.h"

namespace fy {
    void World::step(float deltaTime) {
        for (int i = 0; i < iterations; ++i) {
            // collisions
            checkForCollision();

            // apply forces
            applyForces(deltaTime);
        }
    }

    void World::checkForCollision() {
        for (int i = 0; i < bodies.size(); ++i) {
            for (int j = i + 1; j < bodies.size(); ++j) {
                Vec2f normal;
                float depth;

                Body *body1 = bodies[i];
                Body *body2 = bodies[j];

                if (body1->isStatic && body2->isStatic) {
                    continue;
                }

                if (Collision::intersects(body1, body2, normal, depth)) {
                    // move out of overlap
                    if (body1->isStatic) {
                        body2->move(normal * depth);
                    } else if (body2->isStatic) {
                        body1->move(-normal * depth);
                    } else {
                        body1->move(-(normal * (depth / 2.0f)));
                        body2->move(normal * (depth / 2.0f));
                    }

                    // simulate collision response
                    solveCollision(body1, body2, normal, depth);
                }
            }
        }
    }

    void World::solveCollision(Body *body1, Body *body2, Vec2f normal, float depth) {
        // calculate the velocity between the two bodies
        Vec2f relativeVelocity = body2->velocity - body1->velocity;
        // determine the velocity along the normal
        float velocityAlongNormal = Vec2f::dot(relativeVelocity, normal);

        // if velocity along the normal is greater than 0, the bodies are moving apart
        // -> we don't need to resolve collision
        if (velocityAlongNormal > 0) {
            return;
        }

        // take the smaller restitution
        float restitution = std::min(body1->restitution, body2->restitution);

        // calculate the impulse scalar to change the velocities
        float j = -(1.0f + restitution) * velocityAlongNormal;
        j /= body1->getInverseMass() + body2->getInverseMass();

        // multiply the normal by the scalar to get the impulse
        Vec2f impulse = j * normal;

        // add velocity to both bodies
        body1->velocity -= impulse * body1->getInverseMass();
        body2->velocity += impulse * body2->getInverseMass();
    }

    void World::applyForces(float deltaTime) {
        for (int i = 0; i < (int) bodies.size(); ++i) {
            bodies[i]->step(deltaTime, gravity);
        }
    }
}
