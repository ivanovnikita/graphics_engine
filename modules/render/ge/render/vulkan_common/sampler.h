#pragma once

#include "device.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::UniqueSampler create_sampler
    (
        const DeviceData&,
        const uint32_t mip_levels
    );
}
