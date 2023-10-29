#pragma once

#include "device.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::UniqueSampler create_sampler
    (
        const DeviceData&
    );
}
