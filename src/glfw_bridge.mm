#include "glfw_bridge.h"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <Metal/Metal.h>
#include <QuartzCore/QuartzCore.h>

namespace GLFWBridge {
void addLayer(GLFWwindow *window, CA::MetalLayer *layer) noexcept {
    NSWindow *cocoaWindow = glfwGetCocoaWindow(window);
    cocoaWindow.contentView.layer = (__bridge CAMetalLayer *)layer;
    cocoaWindow.contentView.wantsLayer = YES;
}
}  // namespace GLFWBridge
