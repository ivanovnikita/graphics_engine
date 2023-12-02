#pragma once

#include "ge/common/flags.hpp"

#include <vulkan/vulkan.hpp>

#include <string_view>

namespace ge
{
    enum class DeviceFeatures
    {
        SamplerAnisotropy = 1,
        FillModeNonSolid = 1 << 1,
        WideLines = 1 << 2,
        SampleShading = 1 << 3,
    };

    using DeviceFeaturesFlags = Flags<DeviceFeatures>;

    std::string_view to_string_view(DeviceFeatures);

    vk::PhysicalDeviceFeatures to_vk_physical_device_features(const DeviceFeaturesFlags&);

}
