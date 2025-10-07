#pragma once

struct GLFWwindow;

namespace CA {
class MetalLayer;
}

namespace GLFWBridge {
void add_layer(GLFWwindow *window, CA::MetalLayer *layer) noexcept;
}
