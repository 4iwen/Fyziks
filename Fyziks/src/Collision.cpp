#include "Fyziks/Physics/Collision.h"

namespace fy {
    bool Collision::intersects(Body *body1, Body *body2, Vec2f &normal, float &depth) {
        auto polygon1 = body1->castAndCheck<Polygon>();
        auto polygon2 = body2->castAndCheck<Polygon>();
        auto circle1 = body1->castAndCheck<Circle>();
        auto circle2 = body2->castAndCheck<Circle>();

        // find out what shapes we're dealing with
        if (polygon1 && polygon2) {
            return intersectPolygons(polygon1, polygon2, normal, depth);
        } else if (polygon1 && circle2) {
            return intersectPolygonCircle(polygon1, circle2, normal, depth);
        } else if (polygon2 && circle1) {
            return intersectPolygonCircle(polygon2, circle1, normal, depth);
        } else if (circle1 && circle2) {
            return intersectCircles(circle1, circle2, normal, depth);
        }

        // unsupported body type
        return false;
    }

    // check for collision between two polygons
    bool Collision::intersectPolygons(Polygon *pol1, Polygon *pol2, Vec2f &normal, float &depth) {
        // make sure to get rotated/moved vertices
        std::vector<Vec2f> verts1 = pol1->getTranslatedVertices();
        std::vector<Vec2f> verts2 = pol2->getTranslatedVertices();

        // prepare the normal and depth values
        normal = Vec2f();
        depth = std::numeric_limits<float>::max();

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
            projectVertices(verts1, axis, min1, max1);
            projectVertices(verts2, axis, min2, max2);

            if (min1 >= max2 || min2 >= max1) {
                // we know its not a collision -> return false
                return false;
            }

            // calculate the depth
            float axisDepth = std::min(max2 - min1, max1 - min2);
            if (axisDepth < depth) {
                depth = axisDepth;
                normal = axis;
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
            projectVertices(verts1, axis, min1, max1);
            projectVertices(verts2, axis, min2, max2);

            if (min1 >= max2 || min2 >= max1) {
                // we know its not a collision -> return false
                return false;
            }

            // calculate the depth
            float axisDepth = std::min(max2 - min1, max1 - min2);
            if (axisDepth < depth) {
                depth = axisDepth;
                normal = axis;
            }
        }

        // normalize the normal
        normal.normalize();

        // find the real centers of the polygons
        Vec2f center1 = findArithmeticMean(verts1);
        Vec2f center2 = findArithmeticMean(verts2);

        // invert the normal to prevent incorrect movement
        Vec2f direction = center2 - center1;
        if (Vec2f::dot(direction, normal) < 0) {
            normal = -normal;
        }

        return true;
    }

    // find the average / center of an object
    Vec2f Collision::findArithmeticMean(const std::vector<Vec2f> &vertices) {
        float sumX = 0;
        float sumY = 0;

        for (int i = 0; i < vertices.size(); ++i) {
            Vec2f vec = vertices[i];
            sumX += vec.x;
            sumY += vec.y;
        }

        return {sumX / (float) vertices.size(), sumY / (float) vertices.size()};
    }

    // find the min and max by projecting onto axis
    void Collision::projectVertices(const std::vector<Vec2f> &vertices, Vec2f axis, float &min, float &max) {
        // normalize
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

    // check for collision between two circles
    bool Collision::intersectCircles(Circle *cir1, Circle *cir2, Vec2f &normal, float &depth) {
        // prepare the normal and depth values
        normal = Vec2f();
        depth = 0;

        // calculate the distance from cir 1 pos to cir 2 pos
        float distance = Vec2f::distance(cir1->position, cir2->position);

        // check if the distance is less then the added radius of both circles
        // if yes then circles are colliding
        float radii = cir1->radius + cir2->radius;

        if (distance >= radii) {
            return false;
        }

        // calculate the direction in which the circles are going to be pushed apart
        normal = Vec2f::normalize(cir2->position - cir1->position);
        // calculate the push amount
        depth = radii - distance;

        return true;
    }

    // check for collision between polygon and circle
    bool Collision::intersectPolygonCircle(Polygon *pol, Circle *cir, Vec2f &normal, float &depth) {
        // get vertices of the polygon
        std::vector<Vec2f> verts = pol->getTranslatedVertices();

        // prepare the normal and depth values
        normal = Vec2f();
        depth = std::numeric_limits<float>::max();

        // axis that the projection is going to happen on
        Vec2f axis;
        float min1, max1, min2, max2;

        // go through the verts of the polygon
        for (int i = 0; i < verts.size(); ++i) {
            Vec2f v1 = verts[i];
            Vec2f v2 = verts[(i + 1) % verts.size()];

            // get edge vector
            Vec2f edge = v2 - v1;
            // get its normal
            axis = Vec2f(-edge.y, edge.x);

            // project polygon and circle on axis
            projectVertices(verts, axis, min1, max1);
            projectCircle(cir->position, cir->radius, axis, min2, max2);

            // compare for collision
            if (min1 >= max2 || min2 >= max1) {
                return false;
            }

            // calculate the depth
            float axisDepth = std::min(max2 - min1, max1 - min2);
            if (axisDepth < depth) {
                depth = axisDepth;
                normal = axis;
            }
        }

        // find the closest vertex of polygon and take is as an axis
        Vec2f closest = findClosestVertexPolygon(cir->position, verts);
        axis = closest - cir->position;

        // project polygon and circle on axis
        projectVertices(verts, axis, min1, max1);
        projectCircle(cir->position, cir->radius, axis, min2, max2);

        // compare for collision
        if (min1 >= max2 || min2 >= max1) {
            return false;
        }

        // calculate the depth
        float axisDepth = std::min(max2 - min1, max1 - min2);
        if (axisDepth < depth) {
            depth = axisDepth;
            normal = axis;
        }

        // normalize the normal
        normal.normalize();

        // find the real center of the polygon
        Vec2f polygonCenter = findArithmeticMean(verts);

        // invert the normal to prevent incorrect movement
        Vec2f direction = polygonCenter - cir->position;
        if (Vec2f::dot(direction, normal) < 0) {
            normal = -normal;
        }

        return true;
    }

    // projects circle onto an axis and finds its min and max values
    void Collision::projectCircle(Vec2f position, float radius, Vec2f axis, float &min, float &max) {
        // normalize axis
        axis.normalize();

        Vec2f axisRadius = axis * radius;

        // get max and min point on the circle perpendicular to the axis
        Vec2f p1 = position + axisRadius;
        Vec2f p2 = position - axisRadius;

        // project onto axis
        min = Vec2f::dot(p1, axis);
        max = Vec2f::dot(p2, axis);

        // switch values if min > max
        if (min > max) {
            float temp = min;
            min = max;
            max = temp;
        }
    }

    // find the closest vertex to a point
    Vec2f Collision::findClosestVertexPolygon(Vec2f center, const std::vector<Vec2f> &vertices) {
        float min = std::numeric_limits<float>::max();
        Vec2f result;

        // go through verts
        for (int i = 0; i < vertices.size(); ++i) {
            Vec2f vec = vertices[i];
            // calculate distance
            float distance = Vec2f::distance(vec, center);

            // assign if the distance is less that the previous one
            if (distance < min) {
                min = distance;
                result = vertices[i];
            }
        }

        return result;
    }
}