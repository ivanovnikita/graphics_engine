module;

#include <optional>

export module vulkan_common.create_antialiasing;

export import vulkan_common.image;
export import antialiasing;

import extent;
import vulkan_common.vulkan_fwds;
import vulkan_common.device_fwd;

namespace ge
{
    export Antialiasing choose_max
    (
        const PhysicalDeviceData&,
        const Antialiasing& desired,
        const Logger&
    );

    export vk::SampleCountFlagBits sample_count(const Msaa&);
    export vk::SampleCountFlagBits sample_count(const Antialiasing&);

    export std::optional<ImageData> try_create_msaa_data
    (
        const Antialiasing&,
        const DeviceData&,
        const Extent<uint32_t>&,
        const vk::Format&
    );
}
