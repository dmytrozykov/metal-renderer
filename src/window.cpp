#include "window.h"

#include <GLFW/glfw3.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <stdexcept>

#include "glfw_bridge.h"

Window::Window(const std::string &title, int width, int height) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    handle_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!handle_) {
        throw std::runtime_error("Failed to create GLFW window.");
    }
}

void Window::PollEvents() const noexcept { glfwPollEvents(); }

bool Window::ShouldClose() const noexcept {
    if (!handle_) {
        return true;
    }
    return glfwWindowShouldClose(handle_);
}

void Window::AddMetalLayer(const NSPtr<CA::MetalLayer> &layer) const noexcept {
    int width, height;
    glfwGetFramebufferSize(handle_, &width, &height);
    layer->setDrawableSize(CGSizeMake(width, height));

    GLFWBridge::add_layer(handle_, layer.get());
}
