#include "renderer.h"

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
}

Renderer::~Renderer() {
    if (layer) {
        layer->release();
    }
    if (device) {
        device->release();
    }
}

void Renderer::Run() noexcept {
    while (!window->ShouldClose()) {
        window->PollEvents();
    }
}

void Renderer::CreateDevice() noexcept {
    device = MTL::CreateSystemDefaultDevice();
}

void Renderer::CreateWindow() {
    window = std::make_unique<Window>(WindowParameters::title,
                                      WindowParameters::width,
                                      WindowParameters::height);
}

void Renderer::CreateLayer() noexcept {
    layer = CA::MetalLayer::layer();
    layer->setDevice(device);
    layer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    window->AddMetalLayer(layer);
}
