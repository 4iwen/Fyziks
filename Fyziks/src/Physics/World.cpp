#include <algorithm>
#include "Fyziks/Physics/World.h"
#include "Fyziks/Physics/Collision.h"

namespace fy {
    void World::step(float deltaTime) {
        deltaTime /= (float) iterations;

        for (int i = 0; i < iterations; ++i) {
            contactPairs.clear();
            contactPoints.clear();

            // apply forces
            applyForces(deltaTime);

            // check for collisions
            broadPhase();

            // solve collisions
            narrowPhase();
        }
    }

    void World::applyForces(float deltaTime) {
        for (int i = 0; i < bodies.size(); ++i) {
            bodies[i]->step(deltaTime, gravity);
        }
    }

    void World::broadPhase() {
        for (int i = 0; i < bodies.size(); ++i) {
            for (int j = i + 1; j < bodies.size(); ++j) {
                Body *body1 = bodies[i];
                Body *body2 = bodies[j];

                // skip check between bodies that are static
                if (body1->isStatic && body2->isStatic) {
                    continue;
                }

                // skip check between bodies that are not close to each other
                if (!Collision::intersectAABBs(body1->getAABB(), body2->getAABB())) {
                    continue;
                }

                contactPairs.emplace_back(body1, body2);
            }
        }
    }

    void World::narrowPhase() {
        for (int i = 0; i < contactPairs.size(); ++i) {
            ContactPair pair = contactPairs[i];

            Body *body1 = pair.body1;
            Body *body2 = pair.body2;

            Vec2f normal;
            float depth;

            if (Collision::intersects(body1, body2, normal, depth)) {
                // move out of overlap
                separateBodies(body1, body2, normal * depth);

                Vec2f contactPoint1, contactPoint2;
                int contactCount;

                Collision::findContactPoints(body1, body2, contactPoint1, contactPoint2, contactCount);
                CollisionManifold contact(body1, body2,
                                          contactPoint1, contactPoint2, contactCount,
                                          normal, depth);

                contactPoints.emplace_back(contact);

                solveCollision(contact);
            }
        }
    }

    void World::separateBodies(Body *body1, Body *body2, Vec2f mtv) {
        if (body1->isStatic) {
            body2->move(mtv);
        } else if (body2->isStatic) {
            body1->move(-mtv);
        } else {
            body1->move(-mtv / 2.0f);
            body2->move(mtv / 2.0f);
        }
    }

    void World::solveCollision(const CollisionManifold &contact) {
        Body *body1 = contact.body1;
        Body *body2 = contact.body2;
        Vec2f normal = contact.normal;
        int contactCount = contact.contactCount;
        Vec2f contacts[2] = {
                contact.contactPoint1,
                contact.contactPoint2
        };
        Vec2f impulses[2];
        Vec2f r1List[2];
        Vec2f r2List[2];

        // take the smaller restitution
        float restitution = std::min(body1->restitution, body2->restitution);

        for (int i = 0; i < contactCount; ++i) {
            Vec2f r1 = contacts[i] - body1->position;
            Vec2f r2 = contacts[i] - body2->position;

            r1List[i] = r1;
            r2List[i] = r2;

            Vec2f r1Normal = Vec2f(-r1.y, r1.x);
            Vec2f r2Normal = Vec2f(-r2.y, r2.x);

            Vec2f totalVelocity1 = r1Normal * body1->angularVelocity;
            Vec2f totalVelocity2 = r2Normal * body2->angularVelocity;

            // calculate the velocity between the two bodies
            Vec2f relativeVelocity = (body2->velocity + totalVelocity2) - (body1->velocity + totalVelocity1);
            // determine the velocity along the normal
            float relativeVelocityAlongNormal = Vec2f::dot(relativeVelocity, normal);

            // if velocity along the normal is greater than 0, the bodies are moving apart
            // -> we don't need to resolve collision
            if (relativeVelocityAlongNormal > 0) {
                continue;
            }

            float r1NormalDotNormal = Vec2f::dot(r1Normal, normal);
            float r2NormalDotNormal = Vec2f::dot(r2Normal, normal);

            // calculate the impulse scalar to change the velocities
            float j = -(1.0f + restitution) * relativeVelocityAlongNormal;

            float inverseMassSum = body1->getInverseMass() + body2->getInverseMass();

            j /= inverseMassSum +
                 (r1NormalDotNormal * r1NormalDotNormal) * body1->getInverseInertia() +
                 (r2NormalDotNormal * r2NormalDotNormal) * body2->getInverseInertia();
            j /= (float) contactCount;

            // multiply the normal by the scalar to get the impulse
            Vec2f impulse = j * normal;
            impulses[i] = impulse;

        }

        for (int i = 0; i < contactCount; ++i) {
            Vec2f impulse = impulses[i];

            // add velocity to both bodies
            body1->velocity += -impulse * body1->getInverseMass();
            body1->angularVelocity += -Vec2f::cross(r1List[i], impulse) * body1->getInverseInertia();
            body2->velocity += impulse * body2->getInverseMass();
            body2->angularVelocity += Vec2f::cross(r2List[i], impulse) * body2->getInverseInertia();
        }
    }
}
