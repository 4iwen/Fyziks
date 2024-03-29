#pragma once

#include <cmath>
#include <Fyziks/Api.h>

namespace fy {

#define PIf 3.14159265358979323846f

    class FYZIKS_API Misc {
    public:
        static bool nearlyEqual(float lhs, float rhs, float epsilon = 0.0001f) {
            return std::abs(lhs - rhs) <= epsilon;
        }

        static float toRadians(float degrees) {
            return degrees * PIf / 180.0f;
        }
    };
}