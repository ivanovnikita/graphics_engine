#include "ge/render/debug_callback.h"

#include <iostream>

namespace ge
{
    VKAPI_ATTR vk::Bool32 VKAPI_CALL debug_callback
    (
        VkDebugReportFlagsEXT       flags
      , VkDebugReportObjectTypeEXT  /*object_type*/
      , uint64_t                    /*object*/
      , size_t                      /*location*/
      , int32_t                     /*message_code*/
      , const char*                 layer_prefix
      , const char*                 message
      , void*                       /*user_data*/
    )
    {
        if (std::strcmp(layer_prefix, "loader") == 0)
        {
            return VK_FALSE;
        }

        std::string type;
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

        std::cerr << "Type: " << type << "\n"
                  << "Layer: " << layer_prefix << "\n"
                  << "Message: " << message << "\n";

        return VK_FALSE;
    }
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT
(
    VkInstance                                  instance
  , const VkDebugReportCallbackCreateInfoEXT*   pCreateInfo
  , const VkAllocationCallbacks*                pAllocator
  , VkDebugReportCallbackEXT*                   pCallback
)
{
    static const auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>
    (
        vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT")
    );
    return func(instance, pCreateInfo, pAllocator, pCallback);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT
(
    VkInstance                   instance
  , VkDebugReportCallbackEXT     callback
  , const VkAllocationCallbacks* pAllocator
)
{
    static const auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>
    (
        vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT")
    );
    return func(instance, callback, pAllocator);
}
