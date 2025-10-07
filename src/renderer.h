#pragma once

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <memory>

#include "glfw_context.h"
#include "ns_ptr.h"
#include "window.h"

class Renderer {
public:
    Renderer();
    ~Renderer() = default;

    void Run() noexcept;

    Renderer(const Renderer &)            = delete;
    Renderer(Renderer &&)                 = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer &operator=(Renderer &&)      = delete;

private:
    void CreateDevice() noexcept;
    void CreateWindow();
    void CreateLayer() noexcept;
    void CreateDefaultLibrary();
    void CreateVertexBuffer();
    void CreateCommandQueue();
    void CreateRenderPipeline();

    void Render();
    void SendRenderCommand();
    void EncodeRenderCommand(
        const NSPtr<MTL::RenderCommandEncoder> &commandEncoder);

    NSPtr<MTL::Function> LoadShader(const std::string &title) noexcept;

    GLFWContext glfwContext;
    std::unique_ptr<Window> window;

    NSPtr<MTL::Device> device;
    NSPtr<CA::MetalLayer> layer;

    NSPtr<CA::MetalDrawable> drawable;

    NSPtr<MTL::Library> defaultLibrary;
    NSPtr<MTL::CommandQueue> commandQueue;
    NSPtr<MTL::CommandBuffer> commandBuffer;
    NSPtr<MTL::RenderPipelineState> renderPSO;
    NSPtr<MTL::Buffer> vertexBuffer;
};
