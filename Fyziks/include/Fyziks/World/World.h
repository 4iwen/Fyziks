#pragma once

#include "Fyziks/Api.h"
#include "Fyziks/World/Body.h"

#include <vector>

namespace fy {
    class FYZIKS_API World {
    public:
        std::vector<Body> objects;
    };
}