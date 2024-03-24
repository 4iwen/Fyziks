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

        // find the polygon's moment of inertia, using second moment of area
        // from:
        // https://www.physicsforums.com/showthread.php?t=25293
        // https://stackoverflow.com/questions/3329383/calculating-the-moment-of-inertia-for-a-concave-2d-polygon-relative-to-its-orgin
        float calculateInertia() {
            float numerator = 0.0f;
            float denominator = 0.0f;

            for (int i = 0; i < vertices.size(); ++i) {
                Vec2f current = vertices[i];
                Vec2f next = vertices[(i + 1) % vertices.size()];

                numerator += Vec2f::cross(next, current) * (
                        Vec2f::dot(next, next) +
                        Vec2f::dot(next, current) +
                        Vec2f::dot(current, current)
                );
                denominator += Vec2f::cross(next, current);
            }

            return (mass / 6) * (numerator / denominator);
        }
    };
}