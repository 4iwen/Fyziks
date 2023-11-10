#pragma once

#include "Fyziks/Api.h"
#include "Fyziks/Physics/Polygon.h"
#include "Fyziks/Physics/Circle.h"

namespace fy {
    class FYZIKS_API Collision {
    public:
        static bool intersects(fy::Body *body1, fy::Body *body2);

    private:
        static bool intersectPolygons(fy::Polygon *pol1, fy::Polygon *pol2);

        static bool intersectPolygonCircle(fy::Polygon *pol, fy::Circle *cir);

        static bool intersectCircles(fy::Circle *cir1, fy::Circle *cir2);

        static void projectVertices(const std::vector<Vec2f> &vertices, Vec2f axis, float &min, float &max);

        static void projectCircle(Vec2f position, float radius, Vec2f axis, float &min, float &max);

        static Vec2f findClosestVertexPolygon(Vec2f center, const std::vector<Vec2f> &vertices);
    };
}