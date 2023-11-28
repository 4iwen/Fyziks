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

        void addForce(const Vec2f f) {
            this->force += f;
        }

        void addTorque(const float t) {
            this->torque += t;
        }

        void move(const Vec2f vec) {
            this->position += vec;
        }

        void moveTo(const Vec2f to) {
            this->position = to;
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
        T *tryCast() {
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
