#pragma once

#include "ge/common/flags.hpp"

#include <array>
#include <span>
#include <string_view>

namespace ge
{
    enum class DeviceLayer
    {
        VkLayerKhronosValidation = 1
    };

    std::string_view to_string_view(DeviceLayer);

    constexpr size_t DEVICE_LAYER_MAX_COUNT = 1;
    using DeviceLayersStorage = std::array<const char*, DEVICE_LAYER_MAX_COUNT>;

    using DeviceLayerFlags = Flags<DeviceLayer>;

    std::span<const char*> get_required_layers
    (
        DeviceLayersStorage& output,
        const DeviceLayerFlags&
    ) noexcept;
}
