#pragma once

struct FyVec2 {
    float x, y;

    FyVec2() : x(0), y(0) {}

    FyVec2(float x, float y) : x(x), y(y) {}

    FyVec2 operator+(FyVec2 &other) {
        return FyVec2(this->x + other.x, this->y + other.y);
    }

    FyVec2 operator-(FyVec2 &other) {
        return FyVec2(this->x + other.x, this->y + other.y);
    }

    FyVec2 operator*(FyVec2 &other) {
        return FyVec2(this->x * other.x, this->y * other.y);
    }

    FyVec2 operator/(FyVec2 &other) {
        return FyVec2(this->x / other.x, this->y / other.y);
    }
};