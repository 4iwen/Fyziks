#include <algorithm>
#include "Fyziks/Math/PolygonHelper.h"

namespace fy {
    std::vector<std::vector<Vec2f>> PolygonHelper::triangulate(std::vector<Vec2f> vertices) {
        std::vector<std::vector<Vec2f>> triangles;

        // check if the polygon is cw
        if (!isClockwise(vertices)) {
            // reverse to be cw
            std::reverse(vertices.begin(), vertices.end());
        }

        // repeat until there is only one triangle left
        while (vertices.size() > 3) {
            bool earFound = false;

            // go through all vertices
            for (int i = 0; i < vertices.size(); ++i) {
                // get the previous, current and next indices, preventing out of bounds exception
                unsigned int prevIndex = (i - 1 + vertices.size()) % vertices.size();
                unsigned int currentIndex = i;
                unsigned int nextIndex = (i + 1) % vertices.size();

                // get the vertices from the indices
                const Vec2f &prev = vertices[prevIndex];
                const Vec2f &current = vertices[currentIndex];
                const Vec2f &next = vertices[nextIndex];

                // if the chosen points are convex and it is an ear
                if (isConvex(prev, current, next) && isEar(vertices, prevIndex, currentIndex, nextIndex)) {
                    // add the triangle
                    triangles.push_back({prev, current, next});
                    // remove the vertex for which happened the check
                    vertices.erase(vertices.begin() + i);
                    earFound = true;
                    break;
                }
            }

            if (!earFound) {
                // no ear found, break to avoid an infinite loop
                break;
            }
        }

        // add last triangle
        triangles.push_back({vertices[0], vertices[1], vertices[2]});

        // return triangulated polygon
        return triangles;
    }

    bool PolygonHelper::isPointInsideTriangle(const Vec2f &point,
                                              const Vec2f &prev, const Vec2f &current, const Vec2f &next) {
        // this uses barycentric coordinates for finding out if point is inside triangle

        /*
        Mathematically, if (alpha, beta, gamma) are the barycentric coordinates of a point with respect to a triangle,
        and (prev.x, prev.y), (current.x, current.y), and (next.x, next.y) are the coordinates of the three vertices (prev, current, and next),
        then the point (x, y) can be expressed as follows:

        point.x = alpha * prev.x + beta * current.x + gamma * next.x
        point.y = alpha * prev.y + beta * current.y + gamma * next.y

        That means that if all the barycentric coordinates of the point are positive, the point is inside the triangle
        */

        Vec2f prevToCurrent = current - prev;
        Vec2f currentToNext = next - current;
        Vec2f nextToPrev = prev - next;

        Vec2f prevToPoint = point - prev;
        Vec2f currentToPoint = point - current;
        Vec2f nextToPoint = point - next;

        float alpha = Vec2f::cross(prevToCurrent, prevToPoint);
        float beta = Vec2f::cross(currentToNext, currentToPoint);
        float gamma = Vec2f::cross(nextToPrev, nextToPoint);

        if (alpha > 0.0f || beta > 0.0f || gamma > 0.0f) {
            return false;
        }

        return true;
    }

    bool PolygonHelper::isClockwise(const std::vector<Vec2f> &vertices) {
        float sum = 0.0;

        // goes through all the vertices and calculates the area of a parallelogram formed by the current and next vectors,
        // adds the area to the sum
        for (int i = 0; i < vertices.size(); ++i) {
            const Vec2f &current = vertices[i];
            const Vec2f &next = vertices[(i + 1) % vertices.size()];
            sum += (next.x - current.x) * (next.y + current.y);
        }

        // if sum is larger than 0, the polygon is defined in a clockwise order, otherwise it is in counter-clockwise order
        return sum > 0.0;
    }

    bool PolygonHelper::isEar(const std::vector<Vec2f> &vertices,
                              unsigned int prevIndex, unsigned int currentIndex, unsigned int nextIndex) {
        // get the previous, current and next points of the polygon
        const Vec2f &prev = vertices[prevIndex];
        const Vec2f &current = vertices[currentIndex];
        const Vec2f &next = vertices[nextIndex];

        // check if no other vertices are inside the triangle formed by the vertex and its neighbours
        for (int j = 0; j < vertices.size(); ++j) {
            // check all the vertices except the current and its neighbours (previous and next)
            if (j != prevIndex && j != currentIndex && j != nextIndex) {
                // get the point that we want to test if it is inside the triangle
                const Vec2f &point = vertices[j];

                if (isPointInsideTriangle(point, prev, current, next)) {
                    return false; // the triangle has a point inside -> it is not an ear
                }
            }
        }

        return true; // no points are inside the triangle -> it is an ear
    }

    bool PolygonHelper::isConvex(const Vec2f &prev, const Vec2f &current, const Vec2f &next) {
        // calculate the edges connecting to the current point
        Vec2f prevEdge = prev - current;
        Vec2f nextEdge = next - current;

        // if the cross product is positive, the angle is convex
        return Vec2f::cross(prevEdge, nextEdge) > 0.0f;
    }

    float PolygonHelper::calculateTriangleArea(const Vec2f &point1, const Vec2f &point2, const Vec2f &point3) {
        float area = 0.5f * std::abs(
                point1.x * (point2.y - point3.y) + point2.x * (point3.y - point1.y) + point3.x * (point1.y - point2.y)
        );
        return area;
    }

    Vec2f PolygonHelper::calculateCentroid(const std::vector<Vec2f> &vertices) {
        float area = 0;
        float centroidX = 0;
        float centroidY = 0;

        int j = (int) vertices.size() - 1;

        for (int i = 0; i < vertices.size(); i++) {
            float secondFactor = (vertices[j].x * vertices[i].y) - (vertices[i].x * vertices[j].y);

            area += secondFactor;

            centroidX += (vertices[j].x + vertices[i].x) * secondFactor;
            centroidY += (vertices[j].y + vertices[i].y) * secondFactor;

            j = i;
        }

        area *= 0.5;
        centroidX /= (6.0f * area);
        centroidY /= (6.0f * area);

        return {centroidX, centroidY};
    }
}
