#pragma once

#include "Fyziks/Api.h"
#include "Body.h"
#include "Fyziks/Math/Mat2x2.h"

#include <utility>
#include <vector>
#include "Fyziks/Math/PolygonHelper.h"

namespace fy {
    class FYZIKS_API Polygon : public Body {
    public:
        Polygon(std::vector<Vec2f> vertices) {
            setVertices(std::move(vertices));

            this->mass = calculateMass();
            this->inertia = calculateInertia();
        }

        std::vector<Vec2f> getTransformedVertices() {
            Mat2x2 rotation(this->rotation);

            std::vector<Vec2f> result = this->vertices;

            for (int i = 0; i < result.size(); ++i) {
                result[i] = this->position + rotation * result[i];
            }

            return result;
        }

        std::vector<Vec2f> getVertices() {
            return vertices;
        }

        std::vector<std::vector<Vec2f>> getTransformedTriangles() {
            Mat2x2 rotation(this->rotation);

            std::vector<std::vector<Vec2f>> result = this->triangles;

            for (int i = 0; i < result.size(); ++i) {
                for (int j = 0; j < result[i].size(); ++j) {
                    result[i][j] = this->position + rotation * result[i][j];
                }
            }

            return result;
        }

        std::vector<std::vector<Vec2f>> getTriangles() {
            return triangles;
        }

        void setVertices(std::vector<Vec2f> verts) {
            this->triangles = PolygonHelper::triangulate(verts);
            this->vertices = std::move(verts);
            getAABB();
        }

    private:
        std::vector<Vec2f> vertices;
        std::vector<std::vector<Vec2f>> triangles;

        AABB getAABB() override {
            auto transformedVertices = getTransformedVertices();

            if (transformedVertices.empty()) {
                return {};
            }

            Vec2f min = transformedVertices[0];
            Vec2f max = transformedVertices[0];

            for (int i = 0; i < transformedVertices.size(); ++i) {
                auto vertex = transformedVertices[i];

                if (vertex.x < min.x) {
                    min.x = vertex.x;
                }
                if (vertex.y < min.y) {
                    min.y = vertex.y;
                }
                if (vertex.x > max.x) {
                    max.x = vertex.x;
                }
                if (vertex.y > max.y) {
                    max.y = vertex.y;
                }
            }


            return {min, max};
        }

        float calculateMass() {
            float totalArea = 0.0f;

            auto tris = this->getTriangles();

            for (int i = 0; i < tris.size(); ++i) {
                Vec2f point1 = tris[i][0];
                Vec2f point2 = tris[i][1];
                Vec2f point3 = tris[i][2];

                totalArea += PolygonHelper::calculateTriangleArea(point1, point2, point3);
            }

            return density * totalArea;
        }

        float calculateInertia() {
            float totalInertia = 0.0f;

            auto tris = this->getTriangles();

            for (int i = 0; i < tris.size(); ++i) {
                // get triangle points
                Vec2f point1 = tris[i][0];
                Vec2f point2 = tris[i][1];
                Vec2f point3 = tris[i][2];

                // get the centroid of the triangles
                Vec2f centroid = (point1 + point2 + point3) / 3.0f;

                // calculate the area
                float area = PolygonHelper::calculateTriangleArea(point1, point2, point3);

                // calculate the mass of triangle
                float mass = density * area;

                // calculate the distance from the centroid of triangle to the position of polygon
                float distance = Vec2f::distance(centroid, this->position);

                // add the inertia of one triangle
                totalInertia += mass * distance * distance;
            }

            Vec2f centroidOfPolygon = PolygonHelper::calculateCentroid(this->getVertices());

            float distToCenterOfMass = Vec2f(this->position - centroidOfPolygon).length();
            totalInertia += this->mass * distToCenterOfMass * distToCenterOfMass;

            return totalInertia;
        }
    };
}