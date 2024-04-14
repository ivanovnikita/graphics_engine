module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

#include <iostream>

export module vulkan_common.debug_callback;

export import logger;

import vulkan_common.exception;

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

module : private;

namespace ge
{
    VKAPI_ATTR vk::Bool32 VKAPI_CALL debug_callback
    (
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT /*object_type*/,
        uint64_t /*object*/,
        size_t /*location*/,
        int32_t /*message_code*/,
        const char* layer_prefix,
        const char* message,
        void* user_data
    )
    {
        if (std::string_view{layer_prefix} == "loader")
        {
            return VK_FALSE;
        }

        const char* type = "";
        switch (static_cast<vk::DebugReportFlagBitsEXT>(flags))
        {
        case vk::DebugReportFlagBitsEXT::eDebug:
            type = "DEBUG";
            break;
        case vk::DebugReportFlagBitsEXT::eError:
            type = "ERROR";
            break;
        case vk::DebugReportFlagBitsEXT::eInformation:
            type = "INFO";
            break;
        case vk::DebugReportFlagBitsEXT::ePerformanceWarning:
            type = "PERFOMANCE";
            break;
        case vk::DebugReportFlagBitsEXT::eWarning:
            type = "WARNING";
            break;
        }

        if (user_data != nullptr)
        {
            const Logger& logger = *reinterpret_cast<const Logger*>(user_data);
            logger.log
            (
                LogType::Error,
                "Type: ", type, "\n"
                "Layer: ", layer_prefix, "\n"
                "Message: ", message, "\n"
            );
        }

        return VK_FALSE;
    }

    vk::UniqueDebugReportCallbackEXT init_default_debug_callback
    (
        [[ maybe_unused ]] const vk::Instance& instance,
        [[ maybe_unused ]] const Logger& logger
    )
    {
#ifdef GE_DEBUG_LAYERS_ENABLED
        const vk::DebugReportCallbackCreateInfoEXT create_info
        (
            vk::DebugReportFlagBitsEXT::eError |
            vk::DebugReportFlagBitsEXT::ePerformanceWarning |
            vk::DebugReportFlagBitsEXT::eWarning,
            debug_callback,
            reinterpret_cast<void*>(&const_cast<Logger&>(logger))
        );

        vk::DebugReportCallbackEXT callback;
        const vk::Result result = instance.createDebugReportCallbackEXT(&create_info, nullptr, &callback);
        switch (result)
        {
        case vk::Result::eSuccess:
             break;
        case vk::Result::eErrorOutOfHostMemory:
            GE_THROW_EXPECTED_RESULT(result, "Debug callback creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Debug callback creation failed");
        }
        }

        return vk::UniqueDebugReportCallbackEXT{std::move(callback), {instance}};
#else
        return vk::UniqueDebugReportCallbackEXT{};
#endif
    }
}
