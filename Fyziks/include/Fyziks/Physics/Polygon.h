#pragma once

#include "Fyziks/Api.h"
#include "Body.h"
#include "Fyziks/Math/Mat2x2.h"

#include <vector>

namespace fy {
    class FYZIKS_API Polygon : public Body {
    public:
        std::vector<Vec2f> vertices;

        Polygon(std::vector<Vec2f> vertices) : vertices(std::move(vertices)), Body() {}

        std::vector<Vec2f> getTranslatedVertices() const {
            Mat2x2 rotation(this->rotation);

            std::vector<Vec2f> result = this->vertices;

            for (int i = 0; i < result.size(); ++i) {
                result[i] = this->position + rotation * result[i];
            }

            return result;
        }
    };
}