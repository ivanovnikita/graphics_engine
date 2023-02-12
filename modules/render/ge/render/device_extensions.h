#pragma once

#include "ge/common/flags.hpp"

#include <array>
#include <span>
#include <string_view>

namespace ge
{
    enum class DeviceExtension
    {
        VkKhrSwapchain = 1,
    };

    std::string_view to_string_view(DeviceExtension);

    constexpr size_t DEVICE_EXTENSION_MAX_COUNT = 1;
    using DeviceExtensionStorage = std::array<const char*, DEVICE_EXTENSION_MAX_COUNT>;

    using DeviceExtensionFlags = Flags<DeviceExtension>;

    std::span<const char*> get_required_extensions
    (
        DeviceExtensionStorage& output,
        const DeviceExtensionFlags&
    ) noexcept;
}
