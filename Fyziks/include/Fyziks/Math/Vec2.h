#pragma once

#include "Fyziks/Api.h"
#include <cmath>

namespace fy {
    class FYZIKS_API Vec2 {
    public:
        float x, y;

        Vec2() : x(0.0f), y(0.0f) {}

        Vec2(float x, float y) : x(x), y(y) {}

        Vec2 operator-() const {
            return Vec2(-this->x, -this->y);
        }

        void operator+=(const Vec2 &other) {
            this->x += other.x;
            this->y += other.y;
        }

        void operator-=(const Vec2 &other) {
            this->x -= other.x;
            this->y -= other.y;
        }

        void operator*=(const Vec2 &other) {
            this->x *= other.x;
            this->y *= other.y;
        }

        void operator/=(const Vec2 &other) {
            this->x /= other.x;
            this->y /= other.y;
        }

        float length() {
            return sqrtf(this->x * this->x + this->y * this->y);
        }
    };

    inline Vec2 operator+(const Vec2 &vec1, const Vec2 &vec2) {
        return Vec2(vec1.x + vec2.x, vec1.y + vec2.y);
    }

    inline Vec2 operator-(const Vec2 &vec1, const Vec2 &vec2) {
        return Vec2(vec1.x - vec2.x, vec1.y - vec2.y);
    }

    inline Vec2 operator*(const Vec2 vec, float scalar) {
        return Vec2(vec.x * scalar, vec.y * scalar);
    }
}
