#pragma once

#include "Fyziks/Api.h"
#include "Body.h"

namespace fy {
    class FYZIKS_API Rectangle : public Body {
    public:
        float width, height;

        Rectangle(float width, float height, fy::Vec2 position, float rotation);
    };
}