#include "sample_counts.h"

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
