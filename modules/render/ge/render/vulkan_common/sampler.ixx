module;

#include "vulkan_fwds.h"

export module vulkan_common.sampler;

export import vulkan_common.device;

namespace ge
{
    export vk::UniqueSampler create_sampler
    (
        const DeviceData&,
        const uint32_t mip_levels
    );
}
