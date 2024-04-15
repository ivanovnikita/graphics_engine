module;

#include "vulkan_fwds.h"

#include <vulkan/vulkan.h>

export module vulkan_common.debug_callback;

export import logger;

namespace ge
{
    export VKAPI_ATTR vk::Bool32 VKAPI_CALL debug_callback
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

    export vk::UniqueDebugReportCallbackEXT init_default_debug_callback(const vk::Instance&, const Logger&);
}
