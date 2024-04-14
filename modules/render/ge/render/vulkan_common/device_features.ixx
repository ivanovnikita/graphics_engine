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

module : private;

namespace ge
{
    std::string_view to_string_view(const DeviceFeatures feature)
    {
        switch (feature)
        {
        case DeviceFeatures::SamplerAnisotropy: return "SamplerAnisotropy";
        case DeviceFeatures::FillModeNonSolid: return "FillModeNonSolid";
        case DeviceFeatures::WideLines: return "WideLines";
        case DeviceFeatures::SampleShading: return "SampleShading";
        }

        __builtin_unreachable();
    }

    vk::PhysicalDeviceFeatures to_vk_physical_device_features(const DeviceFeaturesFlags& flags)
    {
        vk::PhysicalDeviceFeatures result;

        if (flags.test(DeviceFeatures::SamplerAnisotropy))
        {
            result.setSamplerAnisotropy(VK_TRUE);
        }

        if (flags.test(DeviceFeatures::FillModeNonSolid))
        {
            result.setFillModeNonSolid(VK_TRUE);
        }

        if (flags.test(DeviceFeatures::WideLines))
        {
            result.setWideLines(VK_TRUE);
        }

        if (flags.test(DeviceFeatures::SampleShading))
        {
            result.setSampleRateShading(VK_TRUE);
        }

        return result;
    }
}
