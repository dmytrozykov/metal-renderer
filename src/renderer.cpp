#include "renderer.h"

#include <simd/simd.h>

#include <QuartzCore/QuartzCore.hpp>
#include <iostream>
#include <string>

namespace WindowParameters {
constexpr std::string kTitle = "Metal Renderer";
constexpr int kWidth         = 800;
constexpr int kHeight        = 600;
}  // namespace WindowParameters

Renderer::Renderer() {
    CreateDevice();
    CreateWindow();
    CreateLayer();
    CreateDefaultLibrary();
    CreateVertexBuffer();
    CreateCommandQueue();
    CreateRenderPipeline();
}

void Renderer::Run() noexcept {
    while (!window_->ShouldClose()) {
        drawable_ = make_ns_ptr(layer_->nextDrawable());
        Render();
        window_->PollEvents();
    }
}

void Renderer::CreateDevice() {
    device_ = make_ns_ptr(MTL::CreateSystemDefaultDevice());
    if (!device_) {
        throw std::runtime_error("Failed to create a device.");
    }
}

void Renderer::CreateWindow() {
    window_ = std::make_unique<Window>(WindowParameters::kTitle,
                                       WindowParameters::kWidth,
                                       WindowParameters::kHeight);
}

void Renderer::CreateLayer() {
    assert(device_ != nullptr);

    layer_ = make_ns_ptr(CA::MetalLayer::layer());
    if (!layer_) {
        throw std::runtime_error("Failed to create a layer.");
    }

    layer_->setDevice(device_.get());
    layer_->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    window_->AddMetalLayer(layer_);
}

void Renderer::CreateDefaultLibrary() {
    default_library_ = make_ns_ptr(device_->newDefaultLibrary());
    if (!default_library_) {
        throw std::runtime_error("Failed to create a default library.");
    }
}

void Renderer::CreateVertexBuffer() {
    const simd::float3 vertices[] = {
        {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}};

    vertex_buffer_ = make_ns_ptr(device_->newBuffer(
        &vertices, sizeof(vertices), MTL::ResourceStorageModeShared));
    if (!vertex_buffer_) {
        throw std::runtime_error("Failed to create a vertex buffer.");
    }
}

void Renderer::CreateCommandQueue() {
    assert(device_ != nullptr);

    command_queue_ = make_ns_ptr(device_->newCommandQueue());
    if (!command_queue_) {
        throw std::runtime_error("Failed to create a command queue.");
    }
}

void Renderer::CreateRenderPipeline() {
    const auto render_pipeline_desc =
        make_ns_ptr(MTL::RenderPipelineDescriptor::alloc()->init());
    if (!render_pipeline_desc) {
        throw std::runtime_error(
            "Failed to create a render pipeline descriptor.");
    }

    const auto *ns_label =
        NS::String::string("Rendering Pipeline", NS::ASCIIStringEncoding);
    render_pipeline_desc->setLabel(ns_label);

    const auto vertex_shader = LoadShader("vertexShader");
    if (!vertex_shader) {
        throw std::runtime_error("Failed to load a vertex shader.");
    }
    render_pipeline_desc->setVertexFunction(vertex_shader.get());

    const auto fragment_shader = LoadShader("fragmentShader");
    if (!fragment_shader) {
        throw std::runtime_error("Failed to load a fragment shader.");
    }
    render_pipeline_desc->setFragmentFunction(fragment_shader.get());

    assert(layer_ != nullptr);
    const MTL::PixelFormat pixel_format = layer_->pixelFormat();
    render_pipeline_desc->colorAttachments()->object(0)->setPixelFormat(
        pixel_format);

    NS::Error *error;
    assert(device_ != nullptr);
    render_pso_ = make_ns_ptr(
        device_->newRenderPipelineState(render_pipeline_desc.get(), &error));
    if (error) {
        std::cerr << "Error: " << error << std::endl;
        error->release();
    }
    if (!render_pso_) {
        throw std::runtime_error("Failed to create a render pipeline.");
    }
}

NSPtr<MTL::Function> Renderer::LoadShader(const std::string &title) noexcept {
    assert(default_library_ != nullptr);
    const auto *ns_function_name =
        NS::String::string(title.c_str(), NS::ASCIIStringEncoding);
    return make_ns_ptr(default_library_->newFunction(ns_function_name));
}

void Renderer::Render() { SendRenderCommand(); }

void Renderer::SendRenderCommand() {
    assert(command_queue_ != nullptr);
    command_buffer_ = make_ns_ptr(command_queue_->commandBuffer());
    if (!command_buffer_) {
        throw std::runtime_error("Failed to create a command buffer.");
    }

    const auto render_pass_desc =
        make_ns_ptr(MTL::RenderPassDescriptor::alloc()->init());
    MTL::RenderPassColorAttachmentDescriptor *color_attach =
        render_pass_desc->colorAttachments()->object(0);
    color_attach->setTexture(drawable_->texture());
    color_attach->setLoadAction(MTL::LoadActionClear);
    color_attach->setClearColor(
        MTL::ClearColor(41.0f / 255.0f, 42.0f / 255.0f, 48.0f / 255.0f, 1.0));
    color_attach->setStoreAction(MTL::StoreActionStore);

    const auto command_encoder = make_ns_ptr(
        command_buffer_->renderCommandEncoder(render_pass_desc.get()));
    if (!command_encoder) {
        throw std::runtime_error("Failed to create a command encoder.");
    }

    EncodeRenderCommand(command_encoder);
    command_encoder->endEncoding();

    command_buffer_->presentDrawable(drawable_.get());
    command_buffer_->commit();
    command_buffer_->waitUntilCompleted();
}

void Renderer::EncodeRenderCommand(
    const NSPtr<MTL::RenderCommandEncoder> &command_encoder) {
    assert(render_pso_ != nullptr);
    assert(vertex_buffer_ != nullptr);

    command_encoder->setRenderPipelineState(render_pso_.get());
    command_encoder->setVertexBuffer(vertex_buffer_.get(), 0, 0);
    const MTL::PrimitiveType primitive_type = MTL::PrimitiveTypeTriangle;
    const NS::UInteger vertex_start         = 0;
    const NS::UInteger vertex_count         = 3;
    command_encoder->drawPrimitives(primitive_type, vertex_start, vertex_count);
}
