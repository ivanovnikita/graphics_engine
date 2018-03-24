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

    device_capabilities_error::~device_capabilities_error() = default;

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

    overflow_error::~overflow_error() = default;

    const char* overflow_error::what() const noexcept
    {
        return std::overflow_error::what();
    }

    invalid_options::~invalid_options() = default;

    const char* invalid_options::what() const noexcept
    {
        return std::logic_error::what();
    }

    invalid_shader::~invalid_shader() = default;

    const char* invalid_shader::what() const noexcept
    {
        return std::logic_error::what();
    }
}
