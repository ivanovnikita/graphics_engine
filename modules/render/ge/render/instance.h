#pragma once

#include "ge/common/logger.hpp"
#include "ge/render/factory/options.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    struct InstanceData final
    {
        static InstanceData create_default(const factory::options::Instance&, const Logger&);

        vk::UniqueInstance instance;
        uint32_t api_version;
        vk::UniqueDebugReportCallbackEXT debug_callback;
    };
}
