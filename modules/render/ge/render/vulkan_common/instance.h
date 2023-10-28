#pragma once

#include "ge/common/logger.hpp"
#include "instance_layers.h"
#include "instance_extensions.h"
#include "version.h"

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
