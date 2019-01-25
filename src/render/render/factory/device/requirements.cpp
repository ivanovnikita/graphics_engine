#include "render/factory/device/requirements.h"

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    std::vector<const char*> get_required_device_extensions()
    {
        return
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    }
}
