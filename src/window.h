#pragma once

#include <string>

#include "ns_ptr.h"

struct GLFWwindow;

namespace CA {
class MetalLayer;
}

class Window {
public:
    Window(const std::string &title, int width, int height);
    ~Window() = default;

    Window(const Window &)            = delete;
    Window(Window &&)                 = delete;
    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&)      = delete;

    void PollEvents() const noexcept;
    [[nodiscard]] bool ShouldClose() const noexcept;

    void AddMetalLayer(const NSPtr<CA::MetalLayer> &layer) const noexcept;

private:
    GLFWwindow *handle = nullptr;
};
