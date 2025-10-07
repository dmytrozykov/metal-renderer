#include "glfw_context.h"

#include <GLFW/glfw3.h>

#include <stdexcept>

GLFWContext::GLFWContext() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }
}

GLFWContext::~GLFWContext() { glfwTerminate(); }
