#include "App.h"

#include <memory>

int main() {
    std::unique_ptr<App> app = std::make_unique<App>();
    app->run();

    return 0;
}
