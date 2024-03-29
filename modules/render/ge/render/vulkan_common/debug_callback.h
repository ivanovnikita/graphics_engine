#pragma once

#include "ge/common/logger.hpp"

#include <vulkan/vulkan.hpp>

namespace ge
{
    VKAPI_ATTR vk::Bool32 VKAPI_CALL debug_callback
    (
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT object_type,
        uint64_t object,
        size_t location,
        int32_t message_code,
        const char* layer_prefix,
        const char* message,
        void* user_data
    );

    vk::UniqueDebugReportCallbackEXT init_default_debug_callback(const vk::Instance&, const Logger&);
}
