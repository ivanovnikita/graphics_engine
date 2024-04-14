module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.create_antialiasing;

export import vulkan_common.device;
export import vulkan_common.image;
export import antialiasing;

import vulkan_common.sample_counts;
import overloaded;

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
        vk::Format
    );
}

module : private;

namespace ge
{
    Antialiasing choose_max
    (
        const PhysicalDeviceData& device_data,
        const Antialiasing& desired,
        const Logger& logger
    )
    {
        return std::visit
        (
            overloaded
            {
                [] (NoAntialiasing) -> Antialiasing
                {
                    return NoAntialiasing{};
                },
                [&logger, &device_data] (const Msaa& msaa)
                {
                    const Flags<MsaaSamples> available = get_sample_counts(device_data);
                    return convert_to_max_available(msaa, available, logger);
                }
            }
            , desired
        );
    }

    vk::SampleCountFlagBits sample_count(const Msaa& msaa)
    {
        switch (msaa.samples)
        {
        case MsaaSamples::x2: return vk::SampleCountFlagBits::e2;
        case MsaaSamples::x4: return vk::SampleCountFlagBits::e4;
        case MsaaSamples::x8: return vk::SampleCountFlagBits::e8;
        }
        __builtin_unreachable();
    }

    vk::SampleCountFlagBits sample_count(const Antialiasing& antialiasing)
    {
        return std::visit
        (
            overloaded
            {
                [] (NoAntialiasing)
                {
                    return vk::SampleCountFlagBits::e1;
                },
                [] (const Msaa& msaa)
                {
                    return sample_count(msaa);
                }
            }
            , antialiasing
        );
    }

    std::optional<ImageData> try_create_msaa_data
    (
        const Antialiasing& antialiasing,
        const DeviceData& device_data,
        const Extent<uint32_t>& extent,
        const vk::Format format
    )
    {
        return std::visit
        (
            overloaded
            {
                [] (NoAntialiasing) -> std::optional<ImageData>
                {
                    return std::nullopt;
                },
                [&device_data, &extent, &format] (const Msaa& msaa) -> std::optional<ImageData>
                {
                    return ImageData::create
                    (
                        device_data,
                        extent,
                        format,
                        1,
                        sample_count(msaa),
                        vk::ImageTiling::eOptimal,
                        vk::ImageUsageFlagBits::eTransientAttachment |
                        vk::ImageUsageFlagBits::eColorAttachment,
                        vk::MemoryPropertyFlagBits::eDeviceLocal,
                        vk::ImageAspectFlagBits::eColor
                    );
                }
            }
            , antialiasing
        );
    }
}
