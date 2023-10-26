#include "Fyziks/Physics/World.h"

namespace fy {
    void World::step() {
        // this->broadPhase();

        // apply forces
        for (int i = 0; i < (int) bodies.size(); ++i) {
            Body *body = bodies[i];

            if (body->inverseMass() == 0.0f)
                continue;

            body->velocity += timeStep * (gravity + body->inverseMass() * body->force);
            body->angularVelocity += timeStep * body->inverseInertia() * body->torque;
        }

        // move objects
        for (int i = 0; i < (int) bodies.size(); ++i) {
            Body *body = bodies[i];

            body->position += timeStep * body->velocity;
            body->rotation += timeStep * body->angularVelocity;

            body->force = Vec2(0.0f, 0.0f);
            body->torque = 0.0f;
        }
    }

    // !!! O(n^2) !!!
    /*
    void World::broadPhase() {
        for (int i = 0; i < bodies.size(); i++) {
            Body *body1 = this->bodies[i];
            for (int j = i + 1; j < bodies.size(); j++) {
                Body *body2 = this->bodies[j];

                if (body1->inverseMass == 0.0f && body2->inverseMass == 0.0f)
                    continue;

                Arbiter arbiter(body1, body2);
                ArbiterKey arbiterKey(body1, body2);

                if (arbiter.numContacts > 0) {
                    ArbiterIterator iterator = arbiters.find(arbiterKey);
                    if (iterator == arbiters.end()) {
                        arbiters.insert(ArbiterPair(arbiterKey, arbiter));
                    } else {
                        iterator->second.update(arbiter.contacts, arbiter.numContacts)
                    }
                } else {
                    arbiters.erase(arbiterKey);
                }
            }
        }
    }
    */
}