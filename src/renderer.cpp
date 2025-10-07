#include "renderer.h"

#include <simd/simd.h>

#include <QuartzCore/QuartzCore.hpp>
#include <iostream>
#include <string>

namespace WindowParameters {
constexpr std::string title = "Metal Renderer";
constexpr int width         = 800;
constexpr int height        = 600;
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
    while (!window->ShouldClose()) {
        drawable = make_ns_ptr(layer->nextDrawable());
        Render();
        window->PollEvents();
    }
}

void Renderer::CreateDevice() noexcept {
    device = make_ns_ptr(MTL::CreateSystemDefaultDevice());
}

void Renderer::CreateWindow() {
    window = std::make_unique<Window>(WindowParameters::title,
                                      WindowParameters::width,
                                      WindowParameters::height);
}

void Renderer::CreateLayer() noexcept {
    layer = make_ns_ptr(CA::MetalLayer::layer());
    layer->setDevice(device.get());
    layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    window->AddMetalLayer(layer);
}

void Renderer::CreateDefaultLibrary() {
    defaultLibrary = make_ns_ptr(device->newDefaultLibrary());
    if (!defaultLibrary) {
        throw std::runtime_error("Failed to create a default library.");
    }
}

void Renderer::CreateVertexBuffer() {
    const simd::float3 vertices[] = {
        {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}};

    vertexBuffer = make_ns_ptr(device->newBuffer(
        &vertices, sizeof(vertices), MTL::ResourceStorageModeShared));
    if (!vertexBuffer) {
        throw std::runtime_error("Failed to create a vertex buffer.");
    }
}

void Renderer::CreateCommandQueue() {
    commandQueue = make_ns_ptr(device->newCommandQueue());
    if (!commandQueue) {
        throw std::runtime_error("Failed to create a command queue.");
    }
}

void Renderer::CreateRenderPipeline() {
    const auto renderPipelineDesc =
        make_ns_ptr(MTL::RenderPipelineDescriptor::alloc()->init());
    if (!renderPipelineDesc) {
        throw std::runtime_error(
            "Failed to create a render pipeline descriptor.");
    }

    const auto *nsLabel =
        NS::String::string("Rendering Pipeline", NS::ASCIIStringEncoding);
    renderPipelineDesc->setLabel(nsLabel);

    const auto vertexShader = LoadShader("vertexShader");
    if (!vertexShader) {
        throw std::runtime_error("Failed to load a vertex shader.");
    }
    renderPipelineDesc->setVertexFunction(vertexShader.get());

    const auto fragmentShader = LoadShader("fragmentShader");
    if (!fragmentShader) {
        throw std::runtime_error("Failed to load a fragment shader.");
    }
    renderPipelineDesc->setFragmentFunction(fragmentShader.get());

    MTL::PixelFormat pixelFormat = layer->pixelFormat();
    renderPipelineDesc->colorAttachments()->object(0)->setPixelFormat(
        pixelFormat);

    NS::Error *error;
    renderPSO = make_ns_ptr(
        device->newRenderPipelineState(renderPipelineDesc.get(), &error));
    if (!renderPSO) {
        std::cerr << "Error: " << error << std::endl;
        throw std::runtime_error("Failed to create a render pipeline.");
    }
}

NSPtr<MTL::Function> Renderer::LoadShader(const std::string &title) noexcept {
    const auto *nsFunctionName =
        NS::String::string(title.c_str(), NS::ASCIIStringEncoding);
    return make_ns_ptr(defaultLibrary->newFunction(nsFunctionName));
}

void Renderer::Render() { SendRenderCommand(); }

void Renderer::SendRenderCommand() {
    commandBuffer = make_ns_ptr(commandQueue->commandBuffer());
    if (!commandBuffer) {
        throw std::runtime_error("Failed to create a command buffer.");
    }

    const auto renderPassDesc =
        make_ns_ptr(MTL::RenderPassDescriptor::alloc()->init());
    MTL::RenderPassColorAttachmentDescriptor *colorAttach =
        renderPassDesc->colorAttachments()->object(0);
    colorAttach->setTexture(drawable->texture());
    colorAttach->setLoadAction(MTL::LoadActionClear);
    colorAttach->setClearColor(
        MTL::ClearColor(41.0f / 255.0f, 42.0f / 255.0f, 48.0f / 255.0f, 1.0));
    colorAttach->setStoreAction(MTL::StoreActionStore);

    const auto commandEncoder =
        make_ns_ptr(commandBuffer->renderCommandEncoder(renderPassDesc.get()));
    if (!commandEncoder) {
        throw std::runtime_error("Failed to create a command encoder.");
    }

    EncodeRenderCommand(commandEncoder);
    commandEncoder->endEncoding();

    commandBuffer->presentDrawable(drawable.get());
    commandBuffer->commit();
    commandBuffer->waitUntilCompleted();
}

void Renderer::EncodeRenderCommand(
    const NSPtr<MTL::RenderCommandEncoder> &commandEncoder) {
    commandEncoder->setRenderPipelineState(renderPSO.get());
    commandEncoder->setVertexBuffer(vertexBuffer.get(), 0, 0);
    MTL::PrimitiveType primitiveType = MTL::PrimitiveTypeTriangle;
    NS::UInteger vertexStart         = 0;
    NS::UInteger vertexCount         = 3;
    commandEncoder->drawPrimitives(primitiveType, vertexStart, vertexCount);
}
