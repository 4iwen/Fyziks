#include "Fyziks/Physics/Collision.h"

namespace fy {
    bool Collision::intersects(Body *body1, Body *body2) {
        Polygon *polygon1 = body1->castAndCheck<Polygon>();
        Polygon *polygon2 = body2->castAndCheck<Polygon>();
        Circle *circle1 = body1->castAndCheck<Circle>();
        Circle *circle2 = body2->castAndCheck<Circle>();

        if (polygon1 && polygon2) {
            return intersectPolygons(polygon1, polygon2);
        } else if (polygon1 && circle2) {
            return intersectPolygonCircle(polygon1, circle2);
        } else if (polygon2 && circle1) {
            return intersectPolygonCircle(polygon2, circle1);
        } else if (circle1 && circle2) {
            return intersectCircles(circle1, circle2);
        }
    }

    bool Collision::intersectPolygons(Polygon *pol1, Polygon *pol2) {
        // make sure to get rotated/moved vertices
        std::vector<Vec2f> verts1 = pol1->getTranslatedVertices();
        std::vector<Vec2f> verts2 = pol2->getTranslatedVertices();

        // go through vertices of polygon A
        for (int i = 0; i < verts1.size(); ++i) {
            Vec2f v1 = verts1[i];
            Vec2f v2 = verts1[(i + 1) % verts1.size()];

            // get edge vector
            Vec2f edge = v2 - v1;
            // calculate normal
            Vec2f axis(-edge.y, edge.x);

            // project onto axis
            float min1, max1, min2, max2;
            Collision::projectVertices(verts1, axis, min1, max1);
            Collision::projectVertices(verts2, axis, min2, max2);

            if (min1 >= max2 || min2 >= max1) {
                return false;
            }
        }

        // go through vertices of polygon B
        for (int i = 0; i < verts2.size(); ++i) {
            Vec2f v1 = verts2[i];
            Vec2f v2 = verts2[(i + 1) % verts2.size()];

            // get edge vector
            Vec2f edge = v2 - v1;
            // calculate normal
            Vec2f axis(-edge.y, edge.x);

            // project onto axis
            float min1, max1, min2, max2;
            Collision::projectVertices(verts1, axis, min1, max1);
            Collision::projectVertices(verts2, axis, min2, max2);

            if (min1 >= max2 || min2 >= max1) {
                return false;
            }
        }

        return true;
    }

// find the min and max by projecting onto axis
    void Collision::projectVertices(const std::vector<Vec2f> &vertices, Vec2f axis, float &min, float &max) {
        axis.normalize();
        min = Vec2f::dot(vertices[0], axis);
        max = min;

        for (int i = 1; i < vertices.size(); ++i) {
            Vec2f vec = vertices[i];
            // project the vertex on the axis
            float proj = Vec2f::dot(vec, axis);

            // find min and max
            if (proj < min) {
                min = proj;
            } else if (proj > max) {
                max = proj;
            }
        }
    }

    bool Collision::intersectCircles(Circle *cir1, Circle *cir2) {
        // calculate the distance from cir 1 pos to cir 2 pos
        float distance = Vec2f::distance(cir1->position, cir2->position);

        // check if the distance is less then the added radius of both circles
        // if yes then circles are colliding
        float radius12 = cir1->radius + cir2->radius;

        if (distance <= radius12) {
            return true;
        }

        return false;
    }

    bool Collision::intersectPolygonCircle(Polygon *pol, Circle *cir) {
        std::vector<Vec2f> verts = pol->getTranslatedVertices();
        Vec2f axis;
        float min1, max1, min2, max2;

        for (int i = 0; i < verts.size(); ++i) {
            Vec2f v1 = verts[i];
            Vec2f v2 = verts[(i + 1) % verts.size()];

            Vec2f edge = v2 - v1;
            axis = Vec2f(-edge.y, edge.x);

            Collision::projectVertices(verts, axis, min1, max1);
            Collision::projectCircle(cir->position, cir->radius, axis, min2, max2);

            if (min1 >= max2 || min2 >= max1) {
                return false;
            }
        }

        Vec2f closest = findClosestVertexPolygon(cir->position, verts);
        axis = closest - cir->position;
        axis.normalize();

        Collision::projectVertices(verts, axis, min1, max1);
        Collision::projectCircle(cir->position, cir->radius, axis, min2, max2);

        if (min1 >= max2 || min2 >= max1) {
            return false;
        }

        return true;
    }

    void Collision::projectCircle(Vec2f position, float radius, Vec2f axis, float &min, float &max) {
        axis.normalize();
        Vec2f axisRadius = axis * radius;

        Vec2f p1 = position + axisRadius;
        Vec2f p2 = position - axisRadius;

        min = Vec2f::dot(p1, axis);
        max = Vec2f::dot(p2, axis);

        if (min > max) {
            float temp = min;
            min = max;
            max = temp;
        }
    }

    Vec2f Collision::findClosestVertexPolygon(Vec2f center, const std::vector<Vec2f> &vertices) {
        float min = std::numeric_limits<float>::max();
        Vec2f result;

        for (int i = 0; i < vertices.size(); ++i) {
            Vec2f vec = vertices[i];
            float distance = Vec2f::distance(vec, center);

            if (distance < min) {
                min = distance;
                result = vertices[i];
            }
        }

        return result;
    }
}