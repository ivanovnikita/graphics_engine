export module vulkan_common.sample_counts;

export import vulkan_common.device;
export import msaa;
export import flags;

namespace ge
{
    export Flags<MsaaSamples> get_sample_counts(const PhysicalDeviceData&);
}
