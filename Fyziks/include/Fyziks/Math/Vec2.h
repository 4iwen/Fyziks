#pragma once

#include "Fyziks/Api.h"
#include <cmath>
#include <cwchar>

namespace fy {
class FYZIKS_API Vec2 {
public:
  float x, y;

  Vec2();

  Vec2(float x, float y);

  Vec2 operator+(Vec2 &other);

  Vec2 operator-(Vec2 &other);

  Vec2 operator*(Vec2 &other);

  Vec2 operator/(Vec2 &other);

  Vec2 operator*(float other);

  Vec2 operator/(float other);

  Vec2 operator-();

  Vec2 &operator+=(Vec2 &other);

  Vec2 &operator-=(Vec2 &other);

  Vec2 &operator*=(Vec2 &other);

  Vec2 &operator/=(Vec2 &other);

  float length();

  float dot(Vec2 &other);

  float cross(Vec2 &other);
};
} // namespace fy
