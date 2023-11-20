#pragma once

#include "Fyziks/Api.h"
#include "Fyziks/Physics/Polygon.h"
#include "Fyziks/Physics/Circle.h"

namespace fy {
    class FYZIKS_API Collision {
    public:
        static bool intersects(Body *body1, Body *body2, Vec2f &normal, float &depth);

    private:
        static bool intersectPolygons(Polygon *pol1, Polygon *pol2, Vec2f &normal, float &depth);

        static bool intersectPolygonCircle(Polygon *pol, Circle *cir, Vec2f &normal, float &depth, bool swapped);

        static Vec2f findArithmeticMean(const std::vector<Vec2f> &vertices);

        static bool intersectCircles(Circle *cir1, Circle *cir2, Vec2f &normal, float &depth);

        static void projectVertices(const std::vector<Vec2f> &vertices, Vec2f axis, float &min, float &max);

        static void projectCircle(Vec2f position, float radius, Vec2f axis, float &min, float &max);

        static Vec2f findClosestVertexPolygon(Vec2f center, const std::vector<Vec2f> &vertices);
    };
}