#include "ge/render/debug_callback.h"
#include "exception.h"

#include <iostream>

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
                "Type: %s\n"
                "Layer: %s\n"
                "Message: %s\n",
                type,
                layer_prefix,
                message
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

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT
(
    VkInstance instance,
    const VkDebugReportCallbackCreateInfoEXT* create_info,
    const VkAllocationCallbacks* allocator,
    VkDebugReportCallbackEXT* callback
)
{
    static const auto func = [&instance] () noexcept
    {
        const auto result = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>
        (
            vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT")
        );
        if (result == nullptr)
        {
            std::cerr << "Func ptr vkCreateDebugReportCallbackEXT is null" << std::endl;
        }
        return result;
    }();
    assert(func != nullptr);
    return func(instance, create_info, allocator, callback);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT
(
    VkInstance instance,
    VkDebugReportCallbackEXT callback,
    const VkAllocationCallbacks* allocator
)
{
    static const auto func = [instance] () noexcept
    {
        const auto result = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>
        (
            vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT")
        );
        if (result == nullptr)
        {
            std::cerr << "Func ptr vkDestroyDebugReportCallbackEXT is null" << std::endl;
        }
        return result;
    }();
    assert(func != nullptr);
    return func(instance, callback, allocator);
}
