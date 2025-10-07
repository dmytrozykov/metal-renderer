#include "window.h"

#include <GLFW/glfw3.h>

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

void Window::AddMetalLayer(CA::MetalLayer *layer) const noexcept {
    GLFWBridge::addLayer(handle, layer);
}
