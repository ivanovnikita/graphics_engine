#pragma once

#include "device.h"
#include "image.h"

#include "ge/render/antialiasing/antialiasing.h"

namespace ge
{
    Antialiasing choose_max
    (
        const PhysicalDeviceData&,
        const Antialiasing& desired,
        const Logger&
    );

    vk::SampleCountFlagBits sample_count(const Msaa&);
    vk::SampleCountFlagBits sample_count(const Antialiasing&);

    std::optional<ImageData> try_create_msaa_data
    (
        const Antialiasing&,
        const DeviceData&,
        const Extent<uint32_t>&,
        vk::Format
    );
}
