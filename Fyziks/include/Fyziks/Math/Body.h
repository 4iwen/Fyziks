#pragma once

#include <memory>
#include "Fyziks/Api.h"
#include "Fyziks/Math/Vec2f.h"
#include "AABB.h"

namespace fy {
    class FYZIKS_API Body {
    public:
        // state
        Vec2f position;
        float rotation;
        Vec2f velocity;
        float angularVelocity;

        // properties
        bool isStatic;
        float mass;
        float inertia;
        float restitution;
        float staticFriction;
        float dynamicFriction;

        // applied forces
        Vec2f force;
        float torque;

        void step(float deltaTime, Vec2f gravity);

        void addForce(const Vec2f f) {
            this->force += f;
        }

        void addTorque(const float t) {
            this->torque += t;
        }

        void move(const Vec2f vec) {
            this->position += vec;
        }

        void moveTo(const Vec2f pos) {
            this->position = pos;
        }

        void moveTo(float x, float y) {
            this->position = Vec2f(x, y);
        }

        float getInverseMass() const {
            if (!isStatic) {
                return 1.0f / mass;
            }

            return 0;
        }

        float getInverseInertia() const {
            if (!isStatic) {
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

        virtual AABB getAABB() = 0;

        virtual ~Body() = default;

    protected:
        Body();
    };
}
