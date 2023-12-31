#pragma once

#include "Fyziks/Api.h"

namespace fy {
    class FYZIKS_API ContactPair {
    public:
        Body *body1;
        Body *body2;

        ContactPair(Body *body1, Body *body2) : body1(body1), body2(body2) {}
    };
}