#pragma once

#include "Fyziks/Api.h"
#include "Body.h"
#include "CollisionManifold.h"
#include "ContactPair.h"

#include <vector>
#include <map>
#include <memory>

namespace fy {
    class FYZIKS_API World {
    public:
        std::vector<Body *> bodies;
        std::vector<ContactPair> contactPairs;
        std::vector<CollisionManifold> contactPoints;
        Vec2f gravity;
        int iterations;

        World() : gravity(Vec2f(0, 9.81f)), iterations(16) {}

        void step(float deltaTime);

        template<typename T, typename... Args>
        T *create(Args &&... args) {
            T *body = new T(std::forward<Args>(args)...);
            bodies.push_back(body);
            return body;
        }

        bool remove(Body *body) {
            for (auto it = bodies.begin(); it != bodies.end(); ++it) {
                if (*it == body) {
                    delete *it;
                    bodies.erase(it);
                    return true;
                }
            }

            return false;
        }

        void clear() {
            for (auto body: bodies) {
                delete body;
            }
            bodies.clear();
        }

        bool areBodiesColliding(Body *body1, Body *body2) const {
            for (const auto &contact: contactPoints) {
                if ((contact.body1 == body1 && contact.body2 == body2) ||
                    (contact.body1 == body2 && contact.body2 == body1)) {
                    return true;
                }
            }

            return false;
        }

    private:
        void broadPhase();

        void narrowPhase();

        void solveCollision(const CollisionManifold &contact);

        void applyForces(float deltaTime);

        void separateBodies(Body *body1, Body *body2, Vec2f mtv);
    };
}
