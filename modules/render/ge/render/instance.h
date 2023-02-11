#pragma once

#include "ge/common/logger.hpp"
#include "ge/render/version.h"
#include "ge/render/factory/options.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    // TODO: add enum for required extensions and layers; save enabled extensions and layers
    struct InstanceData final
    {
        static InstanceData create_default(const factory::options::Instance&, const Logger&);

        vk::UniqueInstance instance;
        Version api_version;
        vk::UniqueDebugReportCallbackEXT debug_callback;
    };
}
