#pragma once

#include "ge/common/logger.hpp"
#include "ge/render/instance_layers.h"
#include "ge/render/instance_extensions.h"
#include "ge/render/version.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    struct InstanceData final
    {
        static InstanceData create_default
        (
            InstanceLayerFlags,
            InstanceExtensionFlags,
            const Logger&
        );

        vk::UniqueInstance instance;
        Version api_version;
        InstanceLayerFlags enabled_layers;
        InstanceExtensionFlags enabled_extension;
        vk::UniqueDebugReportCallbackEXT debug_callback;
    };
}
