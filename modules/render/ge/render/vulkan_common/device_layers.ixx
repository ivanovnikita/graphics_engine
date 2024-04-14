module;

#include <array>
#include <span>
#include <string_view>

export module vulkan_common.device_layers;

export import flags;

namespace ge
{
    export enum class DeviceLayer
    {
        VkLayerKhronosValidation = 1
    };

    export std::string_view to_string_view(DeviceLayer);

    static constexpr size_t DEVICE_LAYER_MAX_COUNT = 1;
    export using DeviceLayersStorage = std::array<const char*, DEVICE_LAYER_MAX_COUNT>;

    export using DeviceLayerFlags = Flags<DeviceLayer>;

    export std::span<const char*> get_required_layers
    (
        DeviceLayersStorage& output,
        const DeviceLayerFlags&
    ) noexcept;
}
