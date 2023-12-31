#include "App.h"

// entry point
int main() {
    // create app
    App app;
    // try running it
    try {
        app.run();
    } catch (const std::exception &e) {
        printf("Error: %s", e.what());
    }

    return 0;
}
