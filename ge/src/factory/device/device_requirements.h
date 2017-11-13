#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device
{

    inline const auto& get_required_device_extensions()
    {
        static constexpr const char* required_extensions[] =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        return required_extensions;
    }

} // namespace ge::impl::factory::device

