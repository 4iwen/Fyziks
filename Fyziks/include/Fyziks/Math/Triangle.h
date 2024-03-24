#pragma once

#include "Fyziks/Api.h"
#include "Polygon.h"

namespace fy {
    class FYZIKS_API Triangle : public Polygon {
    public:
        Triangle(Vec2f point1, Vec2f point2, Vec2f point3) : point1(point1), point2(point2), point3(point3),
                                                             Polygon({point3, point2, point1}) {}

    private:
        Vec2f point1, point2, point3;

        void setPoint1(Vec2f point) {
            this->point1 = point;
            this->setVertices({point1, point2, point3});
        }

        void setPoint2(Vec2f point) {
            this->point2 = point;
            this->setVertices({point1, point2, point3});
        }

        void setPoint3(Vec2f point) {
            this->point3 = point;
            this->setVertices({point1, point2, point3});
        }

        Vec2f getPoint1() {
            return this->point1;
        }

        Vec2f getPoint2() {
            return this->point2;
        }

        Vec2f getPoint3() {
            return this->point3;
        }
    };
}