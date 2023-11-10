#pragma once

#include "Fyziks/Api.h"
#include "Fyziks/Math/Vec2f.h"

namespace fy {
    class FYZIKS_API Body {
    public:
        // state
        fy::Vec2f position;
        float rotation;
        fy::Vec2f velocity;
        float angularVelocity;

        // properties
        float mass;
        float inertia;
        float friction;

        // applied forces
        fy::Vec2f force;
        float torque;

        bool colliding = true;

        void addForce(fy::Vec2f f) {
            this->force += f;
        }

        void addTorque(float t) {
            this->torque += t;
        }

        float getInverseMass() const {
            if (mass != 0) {
                return 1.0f / mass;
            } else {
                return 0.0f;
            }
        }

        float getInverseInertia() const {
            if (inertia != 0) {
                return 1.0f / inertia;
            } else {
                return 0.0f;
            }
        }

        template<typename T>
        T *castAndCheck() {
            if (dynamic_cast<T *>(this)) {
                return dynamic_cast<T *>(this);
            }
            return nullptr;
        }

        virtual ~Body() {}

    protected:
        Body();
    };
}
