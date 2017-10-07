#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl
{

    inline const auto& get_instance_required_extensions()
    {
        static constexpr const char* required_extensions[] =
        {
            VK_KHR_SURFACE_EXTENSION_NAME
          , VK_KHR_XCB_SURFACE_EXTENSION_NAME
          , VK_EXT_DEBUG_REPORT_EXTENSION_NAME
        };

        return required_extensions;
    }

    inline const auto& get_instance_required_layers()
    {
        static constexpr const char* required_layers[] =
        {
            "VK_LAYER_LUNARG_standard_validation"
        };

        return required_layers;
    }

} // namespace ge::impl
