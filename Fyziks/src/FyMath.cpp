#include "FyMath.h"

FyVec2::FyVec2() : x(0), y(0) {}

FyVec2::FyVec2(float x, float y) : x(x), y(y) {}

FyVec2 FyVec2::operator+(const FyVec2 &other) {
    return {x + other.x, y + other.y};
}

FyVec2 FyVec2::operator-(const FyVec2 &other) {
    return {x - other.x, y - other.y};
}

FyVec2 FyVec2::operator*(const FyVec2 &other) {
    return {x * other.x, y * other.y};
}

FyVec2 FyVec2::operator/(const FyVec2 &other) {
    return {x / other.x, y / other.y};
}

FyVec2 FyVec2::operator*(float other) {
    return {x * other, y * other};
}

FyVec2 FyVec2::operator/(float other) {
    return {x / other, y / other};
}

FyVec2 FyVec2::operator-() {
    return {-x, -y};
}

FyVec2 &FyVec2::operator+=(const FyVec2 &other) {
    x += other.x;
    y += other.y;
    return *this;
}

FyVec2 &FyVec2::operator-=(const FyVec2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

FyVec2 &FyVec2::operator*=(const FyVec2 &other) {
    x *= other.x;
    y *= other.y;
    return *this;
}

FyVec2 &FyVec2::operator/=(const FyVec2 &other) {
    x /= other.x;
    y /= other.y;
    return *this;
}

float FyVec2::length() {
    return sqrtf(x * x + y * y);
}

float FyVec2::dot(const FyVec2 &other) {
    return x * other.x + y * other.y;
}

float FyVec2::cross(const FyVec2 &other) {
    return x * other.y - y * other.x;
}