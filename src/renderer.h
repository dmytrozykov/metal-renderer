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
        const NSPtr<MTL::RenderCommandEncoder> &command_encoder);

    NSPtr<MTL::Function> LoadShader(const std::string &title) noexcept;

    GLFWContext glfw_context_;
    std::unique_ptr<Window> window_;

    NSPtr<MTL::Device> device_;
    NSPtr<CA::MetalLayer> layer_;

    NSPtr<CA::MetalDrawable> drawable_;

    NSPtr<MTL::Library> default_library_;
    NSPtr<MTL::CommandQueue> command_queue_;
    NSPtr<MTL::CommandBuffer> command_buffer_;
    NSPtr<MTL::RenderPipelineState> render_pso_;
    NSPtr<MTL::Buffer> vertex_buffer_;
};
