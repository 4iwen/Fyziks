# Fyziks - 2D Physics Engine

> This project is for educational purposes only, for a more complete and optimized physics engine, please refer to [Box2D](https://box2d.org/)

## Features

- Collision detection & resolution (SAT)
  - Circles & polygons (triangles, rectangles, custom polygons)
- Body properties
  - Position, rotation
  - Velocity, angular velocity
  - Mass, inertia
  - Friction, restitution
  - Force, torque
- World properties
  - Gravity
  - Time step
  - Iterations

## Getting Started

### Get yourself a copy

#### Release

You can find the latest release on the [releases](https://github.com/4iwen/Fyziks/releases) page

#### Building

1. Clone the repository recursively
   - `git clone https://github.com/4iwen/Fyziks --recursive`
2. Install [CMake](https://cmake.org/) and [SFML](https://www.sfml-dev.org/)
3. Install a C++ compiler of your choice
   - Windows
       - [MinGW](https://mingw-w64.org/) or [MSVC](https://visualstudio.microsoft.com/downloads/) (comes with Visual Studio)
   - Linux & macOS
       - gcc
           - `sudo apt install gcc`
           - `brew install gcc`
4. Run `build.sh` or `build_mingw.bat` or `build_visual_studio.bat`
5. Built files are located in the bin/ folder

### Usage

```cpp
#include <Fyziks.h>

using namespace fy;

int main()
{
    // Create a world with a gravity of 0, -9.81 and 10 iterations
    World world;
    world.gravity = Vec2f(0.0f, -9.81f);
    world.iterations = 10;

    // Create a circle with initial properties
    auto body = world->create<Circle>(25.0f);
    body->position = Vec2f(100.0f, 100.0f);
    body->velocity = Vec2f(10.0f, 0.0f);
    body->rotation = Misc::toRadians(45.0f);
    body->restitution = 1.0f;
    body->staticFriction = 0.5f;
    body->dynamicFriction = 0.3f;
    
    while (true)
    {
        // Update the world with a time step
        world.step(1.0f / 60.0f);
    
        // Draw the world
        // ...
    }

    return 0;
}
```

## License

Fyziks is licensed under the MIT License. See [LICENSE](LICENSE) for more information.

*Inspired by [Box2D](https://box2d.org/).*
