#pragma once

#include "Fyziks/Api.h"
#include "Body.h"

#include <vector>
#include <map>
#include <memory>

namespace fy {
    class FYZIKS_API World {
    public:
        std::vector<Body *> bodies;
        Vec2f gravity;
        int iterations;

        World() : gravity(Vec2f(0, 9.81f)), iterations(4) {}

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

    private:
        void checkForCollision();

        void solveCollision(Body *body1, Body *body2, Vec2f normal, float depth);

        void applyForces(float deltaTime);
    };
}
