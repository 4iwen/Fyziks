#pragma once

#include <memory>
#include "Fyziks/Api.h"
#include "Fyziks/Math/Vec2f.h"

namespace fy {
    class FYZIKS_API Body {
    public:
        // state
        Vec2f position;
        float rotation;
        Vec2f velocity;
        float angularVelocity;

        // properties
        float mass;
        float inertia;
        float friction;

        // applied forces
        Vec2f force;
        float torque;

        bool colliding = true;

        void addForce(Vec2f f) {
            this->force += f;
        }

        void addTorque(float t) {
            this->torque += t;
        }

        float getInverseMass() const {
            if (mass != 0) {
                return 1.0f / mass;
            }

            return 0.0f;
        }

        float getInverseInertia() const {
            if (inertia != 0) {
                return 1.0f / inertia;
            }

            return 0.0f;
        }

        template<typename T>
        T* castAndCheck() {
            if (auto ptr = dynamic_cast<T *>(this)) {
                return ptr;
            }
            return nullptr;
        }

        virtual ~Body() = default;

    protected:
        Body();
    };
}
