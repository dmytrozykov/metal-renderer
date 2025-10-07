#pragma once

#include <memory>

template <typename T>
struct NSDeleter {
    void operator()(T *object) const noexcept {
        if (object) {
            object->release();
        }
    }
};

template <typename T>
using NSPtr = std::unique_ptr<T, NSDeleter<T>>;

template <typename T>
inline NSPtr<T> make_ns_ptr(T *object) noexcept {
    return NSPtr<T>(object);
}
