#include <vulkan/vulkan.hpp>

#include <iostream>

#include <cassert>

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
