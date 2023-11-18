#pragma once

#include "Fyziks/Api.h"
#include "Fyziks/Physics/Polygon.h"
#include "Fyziks/Physics/Circle.h"

namespace fy {
    class FYZIKS_API Collision {
    public:
        static bool intersects(Body *body1, Body *body2);

    private:
        static bool intersectPolygons(Polygon *pol1, Polygon *pol2);

        static bool intersectPolygonCircle(Polygon *pol, Circle *cir);

        static bool intersectCircles(Circle *cir1, Circle *cir2);

        static void projectVertices(const std::vector<Vec2f> &vertices, Vec2f axis, float &min, float &max);

        static void projectCircle(Vec2f position, float radius, Vec2f axis, float &min, float &max);

        static Vec2f findClosestVertexPolygon(Vec2f center, const std::vector<Vec2f> &vertices);
    };
}