#pragma once

#include <Fyziks/Api.h>
#include <cmath>

namespace fy {
    class FYZIKS_API Mat2x2 {
    public:
        float r1c1, r1c2;
        float r2c1, r2c2;


        Mat2x2() : r1c1(1.0f), r1c2(0.0f), r2c1(0.0f), r2c2(1.0f) {}

        Mat2x2(float angle) {
            float sinValue = sinf(angle);
            float cosValue = cosf(angle);

            r1c1 = cosValue;
            r1c2 = -sinValue;
            r2c1 = sinValue;
            r2c2 = cosValue;
        }
    };

    inline Vec2 operator*(const fy::Mat2x2 &mat, const Vec2 &vec) {
        float xValue = mat.r1c1 * vec.x + mat.r1c2 * vec.y;
        float yValue = mat.r2c1 * vec.x + mat.r2c2 * vec.y;

        return Vec2(xValue, yValue);
    }

    inline Vec2 operator*(const Vec2 &vec, const Mat2x2 &mat) {
        float xValue = mat.r1c1 * vec.x + mat.r1c2 * vec.y;
        float yValue = mat.r2c1 * vec.x + mat.r2c2 * vec.y;

        return Vec2(xValue, yValue);
    }
}
