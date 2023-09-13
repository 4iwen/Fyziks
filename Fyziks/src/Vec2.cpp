#include "Fyziks/Math/Vec2.h"

namespace fy {
Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2 Vec2::operator+(Vec2 &other) { return {x + other.x, y + other.y}; }

Vec2 Vec2::operator-(Vec2 &other) { return {x - other.x, y - other.y}; }

Vec2 Vec2::operator*(Vec2 &other) { return {x * other.x, y * other.y}; }

Vec2 Vec2::operator/(Vec2 &other) { return {x / other.x, y / other.y}; }

Vec2 Vec2::operator*(float other) { return {x * other, y * other}; }

Vec2 Vec2::operator/(float other) { return {x / other, y / other}; }

Vec2 Vec2::operator-() { return {-x, -y}; }

Vec2 &Vec2::operator+=(Vec2 &other) {
  x += other.x;
  y += other.y;
  return *this;
}

Vec2 &Vec2::operator-=(Vec2 &other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

Vec2 &Vec2::operator*=(Vec2 &other) {
  x *= other.x;
  y *= other.y;
  return *this;
}

Vec2 &Vec2::operator/=(Vec2 &other) {
  x /= other.x;
  y /= other.y;
  return *this;
}

float Vec2::length() { return sqrtf(x * x + y * y); }

float Vec2::dot(Vec2 &other) { return x * other.x + y * other.y; }

float Vec2::cross(Vec2 &other) { return x * other.y - y * other.x; }
} // namespace fy
