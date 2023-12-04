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
        }

        std::vector<Vec2f> getTranslatedVertices() {
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

        std::vector<std::vector<Vec2f>> getTranslatedTriangles() {
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
            auto translatedVertices = getTranslatedVertices();

            if (translatedVertices.empty()) {
                return {};
            }

            Vec2f min = translatedVertices[0];
            Vec2f max = translatedVertices[0];

            for (int i = 0; i < translatedVertices.size(); ++i) {
                auto vertex = translatedVertices[i];

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
    };
}