module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.sample_counts;

export import vulkan_common.device;
export import msaa;
export import flags;

namespace ge
{
    export Flags<MsaaSamples> get_sample_counts(const PhysicalDeviceData&);
}

module : private;

namespace ge
{
    Flags<MsaaSamples> get_sample_counts(const PhysicalDeviceData& devide_data)
    {
        const vk::SampleCountFlags flags =
            devide_data.properties.limits.framebufferColorSampleCounts &
            devide_data.properties.limits.framebufferDepthSampleCounts;

        Flags<MsaaSamples> result;

        if ((flags & vk::SampleCountFlagBits::e8) == vk::SampleCountFlagBits::e8)
        {
            result.set(MsaaSamples::x8);
        }

        if ((flags & vk::SampleCountFlagBits::e4) == vk::SampleCountFlagBits::e4)
        {
            result.set(MsaaSamples::x4);
        }

        if ((flags & vk::SampleCountFlagBits::e2) == vk::SampleCountFlagBits::e2)
        {
            result.set(MsaaSamples::x2);
        }

        return result;
    }
}
