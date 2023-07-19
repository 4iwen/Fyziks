#pragma once

#include <Api.h>
#include <cmath>

class FYZIKS_API FyVec2 {
public:
    float x, y;

    FyVec2();

    FyVec2(float x, float y);

    FyVec2 operator+ (const FyVec2& other);

    FyVec2 operator- (const FyVec2& other);

    FyVec2 operator* (const FyVec2& other);

    FyVec2 operator/ (const FyVec2& other);

    FyVec2 operator* (float other);

    FyVec2 operator/ (float other);

    FyVec2 operator- ();

    FyVec2& operator+= (const FyVec2& other);

    FyVec2& operator-= (const FyVec2& other);

    FyVec2& operator*= (const FyVec2& other);

    FyVec2& operator/= (const FyVec2& other);

    float length();

    float dot(const FyVec2& other);

    float cross(const FyVec2& other);
};
