#pragma once

#include "Fyziks/Api.h"
#include "Fyziks/Math/Vec2.h"

namespace fy {
    class FYZIKS_API Body {
    public:
        fy::Vec2 position;
        float rotation;

    protected:
        Body(fy::Vec2 position, float rotation);
    };
}
