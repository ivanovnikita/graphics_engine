#include "debug_callback.h"

#include <iostream>

namespace ge::impl
{

    VKAPI_ATTR vk::Bool32 VKAPI_CALL debug_callback(VkDebugReportFlagsEXT flags,
                                                    VkDebugReportObjectTypeEXT /*object_type*/,
                                                    uint64_t /*object*/,
                                                    size_t /*location*/,
                                                    int32_t /*message_code*/,
                                                    const char* layer_prefix,
                                                    const char* message,
                                                    void* /*user_data*/)
    {
        if (std::strcmp(layer_prefix, "loader") == 0)
        {
            return VK_FALSE;
        }

        std::string type("[ ");
        if ((flags & static_cast<VkDebugReportFlagBitsEXT>(vk::DebugReportFlagBitsEXT::eDebug)) != 0u)
        {
            type.append("DEBUG ");
        }
        else if ((flags & static_cast<VkDebugReportFlagBitsEXT>(vk::DebugReportFlagBitsEXT::eError)) != 0u)
        {
            type.append("ERROR ");
        }
        else if ((flags & static_cast<VkDebugReportFlagBitsEXT>(vk::DebugReportFlagBitsEXT::eInformation)) != 0u)
        {
            type.append("INFO ");
        }
        else if ((flags & static_cast<VkDebugReportFlagBitsEXT>(vk::DebugReportFlagBitsEXT::ePerformanceWarning)) != 0u)
        {
            type.append("PERFOMANCE ");
        }
        else if ((flags & static_cast<VkDebugReportFlagBitsEXT>(vk::DebugReportFlagBitsEXT::eWarning)) != 0u)
        {
            type.append("WARNING ");
        }
        type.append("]");

        std::cerr << "Type: " << type << "\n"
                  << "Layer: " << layer_prefix << "\n"
                  << "Message: " << message << "\n";

        return VK_FALSE;
    }

} // namespace ge::impl
