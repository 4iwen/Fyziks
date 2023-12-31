#pragma once

#include <Fyziks/Fyziks.h>

using namespace fy;

enum BodyType {
    CIRCLE,
    TRIANGLE,
    RECTANGLE,
    POLYGON,
};

class BodyProperties {
public:
    float rotation = 0.0f;
    Vec2f velocity = Vec2f();
    float angularVelocity = 0.0f;

    bool isStatic = false;
    float mass = 1.0f;
    float restitution = 0.5f;
    float staticFriction = 0.6f;
    float dynamicFriction = 0.4f;

    BodyType bodyType = BodyType::CIRCLE;
};