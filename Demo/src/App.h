#pragma once

#include <string>
#include "Fyziks/Physics/World.h"

using namespace fy;

class App {
public:
    bool paused = false;

    void run();

    static void loadDemo(int i, World *world);

private:
    static void demo1(World *world);

    static void demo2(World *world);

    static void demo3(World *world);
};
