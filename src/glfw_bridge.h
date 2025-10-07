#pragma once

struct GLFWwindow;

namespace CA {
class MetalLayer;
}

namespace GLFWBridge {
void addLayer(GLFWwindow *window, CA::MetalLayer *layer) noexcept;
}
