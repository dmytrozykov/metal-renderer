#include "glfw_bridge.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Metal/Metal.h>
#include <QuartzCore/QuartzCore.h>

namespace GLFWBridge {
void add_layer(GLFWwindow *window, CA::MetalLayer *layer) noexcept {
    NSWindow *cocoa_window = glfwGetCocoaWindow(window);
    cocoa_window.contentView.layer = (__bridge CAMetalLayer *)layer;
    cocoa_window.contentView.wantsLayer = YES;
}
}  // namespace GLFWBridge
