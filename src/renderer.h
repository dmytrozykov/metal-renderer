#pragma once

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <memory>

#include "glfw_context.h"
#include "window.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Run() noexcept;

    Renderer(const Renderer &)            = delete;
    Renderer(Renderer &&)                 = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer &operator=(Renderer &&)      = delete;

private:
    void CreateDevice() noexcept;
    void CreateWindow();
    void CreateLayer() noexcept;

    GLFWContext glfwContext;
    std::unique_ptr<Window> window;

    MTL::Device *device;
    CA::MetalLayer *layer;
};
