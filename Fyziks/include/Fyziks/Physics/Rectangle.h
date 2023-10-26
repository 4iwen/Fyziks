#pragma once

#include "Fyziks/Api.h"
#include "Body.h"

namespace fy {
    class FYZIKS_API Rectangle : public Body {
    public:
        float width, height;

        Rectangle(float width, float height) : width(width), height(height), Body() {}
    };
}