#include "Fyziks/Physics/Collision.h"

namespace fy {
    bool Collision::intersects(Body *body1, Body *body2, Vec2f &normal, float &depth) {
        // try to cast to either polygon or circle for both bodies
        auto polygon1 = body1->tryCast<Polygon>();
        auto polygon2 = body2->tryCast<Polygon>();
        auto circle1 = body1->tryCast<Circle>();
        auto circle2 = body2->tryCast<Circle>();

        // find out what shapes we're dealing with
        if (polygon1 && polygon2) {
            return intersectPolygons(polygon1, polygon2, normal, depth);
        } else if (circle1 && circle2) {
            return intersectCircles(circle1, circle2, normal, depth);
        } else if (polygon1 && circle2) {
            // pass in if the bodies were swapped for correcting the normal
            return intersectPolygonCircle(polygon1, circle2, normal, depth, false);
        } else if (polygon2 && circle1) {
            // pass in if the bodies were swapped for correcting the normal
            return intersectPolygonCircle(polygon2, circle1, normal, depth, true);
        }

        // unsupported body type
        return false;
    }

    // check for collision between two polygons
    bool Collision::intersectPolygons(Polygon *pol1, Polygon *pol2, Vec2f &normal, float &depth) {
        std::vector<std::vector<Vec2f>> triangles1 = pol1->getTransformedTriangles();
        std::vector<std::vector<Vec2f>> triangles2 = pol2->getTransformedTriangles();

        bool isIntersecting = false;
        float maxDepth = std::numeric_limits<float>::min();
        Vec2f maxNormal;

        for (int i = 0; i < triangles1.size(); ++i) {
            for (int j = 0; j < triangles2.size(); ++j) {
                float tempDepth;
                Vec2f tempNormal;
                if (intersectConvexPolygons(triangles1[i], triangles2[j], tempNormal, tempDepth)) {
                    if (tempDepth > maxDepth) {
                        isIntersecting = true;
                        maxDepth = tempDepth;
                        maxNormal = tempNormal;
                    }
                }
            }
        }

        depth = maxDepth;
        normal = maxNormal;

        return isIntersecting;
    }

    bool Collision::intersectConvexPolygons(const std::vector<Vec2f> &verts1, const std::vector<Vec2f> &verts2,
                                            Vec2f &normal, float &depth) {
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
            // normalize the axis
            axis.normalize();

            // project onto axis
            float min1, max1, min2, max2;
            projectVertices(verts1, axis, min1, max1);
            projectVertices(verts2, axis, min2, max2);

            if (min1 >= max2 || min2 >= max1) {
                // we know it's not a collision -> return false
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
            // normalize the axis
            axis.normalize();

            // project onto axis
            float min1, max1, min2, max2;
            projectVertices(verts1, axis, min1, max1);
            projectVertices(verts2, axis, min2, max2);

            if (min1 >= max2 || min2 >= max1) {
                // we know it's not a collision -> return false
                return false;
            }

            // calculate the depth
            float axisDepth = std::min(max2 - min1, max1 - min2);
            if (axisDepth < depth) {
                depth = axisDepth;
                normal = axis;
            }
        }

        // find the real centers of the polygons
        Vec2f center1 = findArithmeticMean(verts1);
        Vec2f center2 = findArithmeticMean(verts2);

        // calculate the direction vector, along which the normal vector should be corrected
        Vec2f direction = center2 - center1;
        // project the direction vector onto the normal vector to see if it needs to be inverted
        if (Vec2f::dot(direction, normal) < 0.0f) {
            // the normal is pointing in the opposite way -> flip it
            normal = -normal;
        }

        return true;
    }

    // find the average / center of an object
    Vec2f Collision::findArithmeticMean(const std::vector<Vec2f> &vertices) {
        // variables for storing the sums of x and y coordinates
        float sumX = 0;
        float sumY = 0;

        // go through all the vertices and add the x and y
        for (int i = 0; i < vertices.size(); ++i) {
            Vec2f vec = vertices[i];
            sumX += vec.x;
            sumY += vec.y;
        }

        // return the arithmetic mean
        return {sumX / (float) vertices.size(), sumY / (float) vertices.size()};
    }

    // find the min and max by projecting onto axis
    void Collision::projectVertices(const std::vector<Vec2f> &vertices, Vec2f axis, float &min, float &max) {
        // prepare variables for storing the min and max values
        min = Vec2f::dot(vertices[0], axis);
        max = min;

        // go through all the vertices
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

        // handle the case where the circles are in the same position
        if (distance == 0) {
            normal = Vec2f(1, 0);
        } else {
            // calculate the direction in which the circles are going to be pushed apart & normalize it
            normal = Vec2f::normalize(cir2->position - cir1->position);
        }


        // calculate the push amount
        depth = radii - distance;

        return true;
    }

    // check for collision between polygon and circle
    bool Collision::intersectPolygonCircle(Polygon *pol, Circle *cir, Vec2f &normal, float &depth, bool swapped) {
        std::vector<std::vector<Vec2f>> triangles = pol->getTransformedTriangles();

        bool isIntersecting = false;
        float maxDepth = -std::numeric_limits<float>::max();
        Vec2f maxNormal;

        for (int i = 0; i < triangles.size(); ++i) {
            float tempDepth;
            Vec2f tempNormal;
            if (intersectConvexPolygonCircle(triangles[i], cir, tempNormal, tempDepth, swapped)) {
                if (tempDepth > maxDepth) {
                    isIntersecting = true;
                    maxDepth = tempDepth;
                    maxNormal = tempNormal;
                }
            }
        }

        depth = maxDepth;
        normal = maxNormal;

        return isIntersecting;
    }

    bool Collision::intersectConvexPolygonCircle(const std::vector<Vec2f> &verts, Circle *cir,
                                                 Vec2f &normal, float &depth, bool swapped) {
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
            // normalize the axis
            axis.normalize();

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
        // normalize the axis
        axis.normalize();

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

        // find the real center of the polygon
        Vec2f polygonCenter = findArithmeticMean(verts);

        // invert the normal to prevent incorrect movement
        Vec2f direction = swapped ? polygonCenter - cir->position : cir->position - polygonCenter;
        // project the direction vector onto the normal vector to decide if the normal vector should be flipped
        if (Vec2f::dot(direction, normal) < 0.0f) {
            // direction and normal are pointing the other directions -> flip the normal
            normal = -normal;
        }

        return true;
    }

    // projects circle onto an axis and finds its min and max values
    void Collision::projectCircle(Vec2f position, float radius, Vec2f axis, float &min, float &max) {
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
        // prepare the variables where to store the min distance and the result
        float min = std::numeric_limits<float>::max();
        Vec2f result;

        // go through all vertices
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

    bool Collision::intersectAABBs(AABB aabb1, AABB aabb2) {
        // simple check for determining if two bodies are close to each other
        // to avoid doing unnecessary calculations
        if (aabb1.max.x <= aabb2.min.x || aabb2.max.x <= aabb1.min.x ||
            aabb1.max.y <= aabb2.min.y || aabb2.max.y <= aabb1.min.y) {
            return false;
        }

        return true;
    }

    void Collision::findContactPoints(Body *body1, Body *body2,
                                      Vec2f &contactPoint1, Vec2f &contactPoint2, int &contactCount) {
        // try to cast to either polygon or circle for both bodies
        auto polygon1 = body1->tryCast<Polygon>();
        auto polygon2 = body2->tryCast<Polygon>();
        auto circle1 = body1->tryCast<Circle>();
        auto circle2 = body2->tryCast<Circle>();

        // find out what shapes we're dealing with
        if (polygon1 && polygon2) {
            findContactPointsPolygons(polygon1, polygon2, contactPoint1, contactPoint2, contactCount);
        } else if (circle1 && circle2) {
            findContactPointCircles(circle1, circle2, contactPoint1);
            contactCount = 1;
        } else if (polygon1 && circle2) {
            findContactPointPolygonCircle(polygon1, circle2, contactPoint1);
            contactCount = 1;
        } else if (polygon2 && circle1) {
            findContactPointPolygonCircle(polygon2, circle1, contactPoint1);
            contactCount = 1;
        }
    }

    // method for finding the contact point between two circles
    void Collision::findContactPointCircles(Circle *cir1, Circle *cir2, Vec2f &contactPoint) {
        // make a vector that points from one circle to the other
        Vec2f direction = cir2->position - cir1->position;

        // normalize it, then multiply it by the circle's radius to get the contact point
        direction.normalize();
        contactPoint = cir1->position + direction * cir1->radius;
    }

    // method for finding the contact point between a polygon and a circle
    void Collision::findContactPointPolygonCircle(Polygon *pol, Circle *cir, Vec2f &contactPoint) {
        // get transformed vertices of the polygon
        auto transformedVertices = pol->getTransformedVertices();

        // variable for storing the smallest distance
        float minDistanceSquared = std::numeric_limits<float>::max();

        for (int i = 0; i < transformedVertices.size(); ++i) {
            // get the line segment
            Vec2f current = transformedVertices[i];
            Vec2f next = transformedVertices[(i + 1) % transformedVertices.size()];

            // temp variables for storing the distance and contact point
            float distanceSquared;
            Vec2f contact;

            // find the contact and distance for the current line segment
            pointSegmentDistance(cir->position, current, next, distanceSquared, contact);

            // if the distance is smaller, we found a new point that is closer
            if (distanceSquared < minDistanceSquared) {
                minDistanceSquared = distanceSquared;
                contactPoint = contact;
            }
        }
    }

    void Collision::findContactPointsPolygons(Polygon *pol1, Polygon *pol2, Vec2f &contactPoint1, Vec2f &contactPoint2,
                                              int &contactCount) {
        auto pol1TransformedVertices = pol1->getTransformedVertices();
        auto pol2TransformedVertices = pol2->getTransformedVertices();

        float minDistanceSquared = std::numeric_limits<float>::max();

        for (int i = 0; i < pol1TransformedVertices.size(); ++i) {
            Vec2f point = pol1TransformedVertices[i];

            for (int j = 0; j < pol2TransformedVertices.size(); ++j) {
                Vec2f current = pol2TransformedVertices[j];
                Vec2f next = pol2TransformedVertices[(j + 1) % pol2TransformedVertices.size()];

                float distanceSquared;
                Vec2f contact;

                pointSegmentDistance(point, current, next, distanceSquared, contact);

                if (Misc::nearlyEqual(distanceSquared, minDistanceSquared)) {
                    if (!Vec2f::nearlyEqual(contact, contactPoint1)) {
                        contactCount = 2;
                        contactPoint2 = contact;
                    }
                } else if (distanceSquared < minDistanceSquared) {
                    minDistanceSquared = distanceSquared;
                    contactCount = 1;
                    contactPoint1 = contact;
                }
            }
        }

        for (int i = 0; i < pol2TransformedVertices.size(); ++i) {
            Vec2f point = pol2TransformedVertices[i];

            for (int j = 0; j < pol1TransformedVertices.size(); ++j) {
                Vec2f current = pol1TransformedVertices[j];
                Vec2f next = pol1TransformedVertices[(j + 1) % pol1TransformedVertices.size()];

                float distanceSquared;
                Vec2f contact;

                pointSegmentDistance(point, current, next, distanceSquared, contact);

                if (Misc::nearlyEqual(distanceSquared, minDistanceSquared)) {
                    if (!Vec2f::nearlyEqual(contact, contactPoint1)) {
                        contactCount = 2;
                        contactPoint2 = contact;
                    }
                } else if (distanceSquared < minDistanceSquared) {
                    minDistanceSquared = distanceSquared;
                    contactCount = 1;
                    contactPoint1 = contact;
                }
            }
        }
    }

    void Collision::pointSegmentDistance(Vec2f point, Vec2f a, Vec2f b, float &distanceSquared, Vec2f &contactPoint) {
        // get the vector of the line segment
        Vec2f ab = b - a;
        // get the vector pointing from a to the point
        Vec2f aPoint = point - a;

        // project aPoint on ab
        float proj = Vec2f::dot(aPoint, ab);
        // get the length squared
        float abLengthSquared = ab.lengthSquared();
        // calculate the distance of the point from the line segment
        float distance = proj / abLengthSquared;

        // figure out the contact point
        if (distance <= 0.0f) {
            contactPoint = a;
        } else if (distance >= 1.0f) {
            contactPoint = b;
        } else {
            contactPoint = a + ab * distance;
        }

        // get the distance squared for figuring out which point is the closest
        distanceSquared = Vec2f::distanceSquared(point, contactPoint);
    }
}