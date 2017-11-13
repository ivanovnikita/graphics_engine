#include "exception.h"

namespace ge
{
    exception::~exception() = default;

    vulkan_error::vulkan_error(const std::string& what)
        : std::runtime_error (what)
    {
    }

    vulkan_error::vulkan_error(const char* what)
        : std::runtime_error (what)
    {
    }

    vulkan_error::~vulkan_error() = default;

    const char* vulkan_error::what() const noexcept
    {
        return std::runtime_error::what();
    }

    device_capabilities_error::device_capabilities_error(const std::string& what)
        : vulkan_error (what)
    {
    }

    device_capabilities_error::device_capabilities_error(const char* what)
        : vulkan_error (what)
    {
    }

    window_error::window_error(const std::string& what)
        : std::runtime_error (what)
    {
    }

    window_error::window_error(const char* what)
        : std::runtime_error (what)
    {
    }

    window_error::~window_error() = default;

    const char* window_error::what() const noexcept
    {
        return std::runtime_error::what();
    }
}
