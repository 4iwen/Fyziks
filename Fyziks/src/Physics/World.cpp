#include <algorithm>
#include "Fyziks/Physics/World.h"
#include "Fyziks/Physics/Collision.h"
#include <stdio.h>

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
        Vec2f contactPoint1 = contact.contactPoint1;
        Vec2f contactPoint2 = contact.contactPoint2;
        int contactCount = contact.contactCount;

        // prepare arrays for storing values for calculation of the collision resolve impulses
        Vec2f contactList[2];
        Vec2f impulseList[2];
        Vec2f frictionImpulseList[2];
        Vec2f r1List[2];
        Vec2f r2List[2];
        float jList[2] = {0};

        // take the smaller restitution
        float restitution = std::min(body1->restitution, body2->restitution);
        float staticFriction = (body1->staticFriction + body2->staticFriction) * 0.5f; // approximation
        float dynamicFriction = (body1->dynamicFriction + body2->dynamicFriction) * 0.5f; // approximation

        // store contact points
        contactList[0] = contactPoint1;
        contactList[1] = contactPoint2;

        for (int i = 0; i < contactCount; ++i) {
            Vec2f r1 = contactList[i] - body1->position;
            Vec2f r2 = contactList[i] - body2->position;

            r1List[i] = r1;
            r2List[i] = r2;

            Vec2f r1Normal = Vec2f(-r1.y, r1.x);
            Vec2f r2Normal = Vec2f(-r2.y, r2.x);

            Vec2f totalVelocity1 = r1Normal * body1->angularVelocity;
            Vec2f totalVelocity2 = r2Normal * body2->angularVelocity;

            // calculate the velocity between the two bodies
            Vec2f relativeVelocity =
                    (body2->velocity + totalVelocity2) -
                    (body1->velocity + totalVelocity1);
            // determine the velocity along the normal
            float relativeVelocityAlongNormal = Vec2f::dot(relativeVelocity, normal);

            // if velocity along the normal is greater than 0, the bodies are moving apart
            // -> we don't need to resolve collision
            if (relativeVelocityAlongNormal > 0.0f) {
                continue;
            }

            float r1NormalDotNormal = Vec2f::dot(r1Normal, normal);
            float r2NormalDotNormal = Vec2f::dot(r2Normal, normal);

            // calculate the impulse scalar to change the velocities
            float j = -(1.0f + restitution) * relativeVelocityAlongNormal;

            float inverseMassSum = body1->getInverseMass() + body2->getInverseMass();
            float denominator = inverseMassSum +
                                (r1NormalDotNormal * r1NormalDotNormal) * body1->getInverseInertia() +
                                (r2NormalDotNormal * r2NormalDotNormal) * body2->getInverseInertia();

            j /= denominator;
            j /= (float) contactCount;

            jList[i] = j;

            // multiply the normal by the scalar to get the impulse
            Vec2f impulse = j * normal;
            impulseList[i] = impulse;
        }

        for (int i = 0; i < contactCount; ++i) {
            Vec2f impulse = impulseList[i];

            // add velocity to both bodies
            body1->velocity += -impulse * body1->getInverseMass();
            body1->angularVelocity += -Vec2f::cross(r1List[i], impulse) * body1->getInverseInertia();

            body2->velocity += impulse * body2->getInverseMass();
            body2->angularVelocity += Vec2f::cross(r2List[i], impulse) * body2->getInverseInertia();
        }

        for (int i = 0; i < contactCount; ++i) {
            Vec2f r1 = contactList[i] - body1->position;
            Vec2f r2 = contactList[i] - body2->position;

            r1List[i] = r1;
            r2List[i] = r2;

            float j = jList[i];

            Vec2f r1Normal = Vec2f(-r1.y, r1.x);
            Vec2f r2Normal = Vec2f(-r2.y, r2.x);

            Vec2f totalVelocity1 = r1Normal * body1->angularVelocity;
            Vec2f totalVelocity2 = r2Normal * body2->angularVelocity;

            // calculate the velocity between the two bodies
            Vec2f relativeVelocity =
                    (body2->velocity + totalVelocity2) -
                    (body1->velocity + totalVelocity1);

            Vec2f tangent = relativeVelocity - Vec2f::dot(relativeVelocity, normal) * normal;
            if (Vec2f::nearlyEqual(tangent, Vec2f())) {
                continue;
            } else {
                tangent.normalize();
            }

            float r1NormalDotTangent = Vec2f::dot(r1Normal, tangent);
            float r2NormalDotTangent = Vec2f::dot(r2Normal, tangent);

            // calculate the impulse scalar to change the velocities
            float jt = -Vec2f::dot(relativeVelocity, tangent);

            float inverseMassSum = body1->getInverseMass() + body2->getInverseMass();
            float denominator = inverseMassSum +
                                (r1NormalDotTangent * r1NormalDotTangent) * body1->getInverseInertia() +
                                (r2NormalDotTangent * r2NormalDotTangent) * body2->getInverseInertia();

            jt /= denominator;
            jt /= (float) contactCount;

            Vec2f frictionImpulse;

            // coulomb's law of friction
            if (std::fabs(jt) <= j * staticFriction) {
                frictionImpulse = jt * tangent;
            } else {
                frictionImpulse = -j * tangent * dynamicFriction;
            }

            frictionImpulseList[i] = frictionImpulse;
        }

        for (int i = 0; i < contactCount; ++i) {
            Vec2f frictionImpulse = frictionImpulseList[i];

            // add friction velocity to both bodies
            body1->velocity += -frictionImpulse * body1->getInverseMass();
            body1->angularVelocity += -Vec2f::cross(r1List[i], frictionImpulse) * body1->getInverseInertia();

            body2->velocity += frictionImpulse * body2->getInverseMass();
            body2->angularVelocity += Vec2f::cross(r2List[i], frictionImpulse) * body2->getInverseInertia();
        }
    }
}
