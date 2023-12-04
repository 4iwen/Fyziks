#pragma once

#include "Fyziks/Api.h"
#include "Fyziks/Physics/Body.h"

#include <vector>

namespace fy {
    class FYZIKS_API CollisionManifold {
    public:
        Body *body1;
        Body *body2;

        Vec2f contactPoint1;
        Vec2f contactPoint2;
        int contactCount;

        Vec2f normal;
        float depth;

        CollisionManifold(Body *body1, Body *body2,
                          Vec2f contactPoint1, Vec2f contactPoint2, int contactCount,
                          Vec2f normal, float depth) :
                body1(body1), body2(body2),
                contactPoint1(contactPoint1), contactPoint2(contactPoint2), contactCount(contactCount),
                normal(normal), depth(depth) {}
    };
}