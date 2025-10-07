#include <iostream>

#include "renderer.h"

int main(int argc, const char *argv[]) {
    try {
        Renderer renderer;
        renderer.Run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
