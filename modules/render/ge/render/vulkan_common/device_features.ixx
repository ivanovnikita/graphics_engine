module;

#include <vulkan/vulkan.hpp>

#include <string_view>

export module vulkan_common.device_features;

export import flags;

namespace ge
{
    export enum class DeviceFeatures
    {
        SamplerAnisotropy = 1,
        FillModeNonSolid = 1 << 1,
        WideLines = 1 << 2,
        SampleShading = 1 << 3,
    };

    export using DeviceFeaturesFlags = Flags<DeviceFeatures>;

    export std::string_view to_string_view(DeviceFeatures);

    export vk::PhysicalDeviceFeatures to_vk_physical_device_features(const DeviceFeaturesFlags&);

}
