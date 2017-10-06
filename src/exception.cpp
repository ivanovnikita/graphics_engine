#include "exception.h"

namespace ge
{
    exception::~exception() = default;

    vulkan_error::vulkan_error(const std::string& what)
        : ge::exception      ()
        , std::runtime_error (what)
    {
    }

    vulkan_error::vulkan_error(const char* what)
        : ge::exception      ()
        , std::runtime_error (what)
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
}
