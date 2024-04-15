module;

#include <array>
#include <span>
#include <string_view>

export module vulkan_common.device_extensions;

import flags;

namespace ge
{
    export enum class DeviceExtension
    {
        VkKhrSwapchain = 1,
    };

    export std::string_view to_string_view(DeviceExtension);

    static constexpr size_t DEVICE_EXTENSION_MAX_COUNT = 1;
    export using DeviceExtensionStorage = std::array<const char*, DEVICE_EXTENSION_MAX_COUNT>;

    export using DeviceExtensionFlags = Flags<DeviceExtension>;

    export std::span<const char*> get_required_extensions
    (
        DeviceExtensionStorage& output,
        const DeviceExtensionFlags&
    ) noexcept;
}
