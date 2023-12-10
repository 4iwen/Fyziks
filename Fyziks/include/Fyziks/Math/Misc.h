#pragma once

#include <cmath>

#define PIf 3.14159265358979323846f

namespace fy {
    class Misc {
    public:
        static bool nearlyEqual(float lhs, float rhs, float epsilon = 0.0001f) {
            return std::abs(lhs - rhs) <= epsilon;
        }
    };
}