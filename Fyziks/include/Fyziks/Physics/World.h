#pragma once

#include "Fyziks/Api.h"
#include "Arbiter.h"
#include "ArbiterKey.h"

#include <vector>
#include <map>

namespace fy {
    class FYZIKS_API World {
    public:
        std::vector<Body *> bodies;
        std::map<ArbiterKey, Arbiter> arbiters;
        fy::Vec2 gravity;
        int iterations;
        float timeStep;

        World() : gravity(fy::Vec2(0, 9.81f)), iterations(8), timeStep(1.0f / 60.0f) {}

        void step();

        void add(Body *body) {
            bodies.push_back(body);
        }

        void clear() {
            bodies.clear();
        }

    private:
        void broadPhase();
    };
}
