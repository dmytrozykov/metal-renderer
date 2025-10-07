#include "window.h"

#include <GLFW/glfw3.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <stdexcept>

#include "glfw_bridge.h"

Window::Window(const std::string &title, int width, int height) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!handle) {
        throw std::runtime_error("Failed to create GLFW window.");
    }
}

void Window::PollEvents() const noexcept { glfwPollEvents(); }

bool Window::ShouldClose() const noexcept {
    if (!handle) {
        return true;
    }
    return glfwWindowShouldClose(handle);
}

void Window::AddMetalLayer(const NSPtr<CA::MetalLayer> &layer) const noexcept {
    int width, height;
    glfwGetFramebufferSize(handle, &width, &height);
    layer->setDrawableSize(CGSizeMake(width, height));

    GLFWBridge::addLayer(handle, layer.get());
}
