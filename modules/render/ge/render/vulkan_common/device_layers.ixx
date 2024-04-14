module;

#include <array>
#include <span>
#include <string_view>

#include <cassert>

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

module : private;

namespace ge
{
    std::string_view to_string_view(const DeviceLayer layer)
    {
        switch (layer)
        {
        case DeviceLayer::VkLayerKhronosValidation: return "VK_LAYER_KHRONOS_validation";
        }

        __builtin_unreachable();
    }

    std::span<const char*> get_required_layers
    (
        DeviceLayersStorage& output,
        const DeviceLayerFlags& flags
    ) noexcept
    {
        size_t layers_count = 0;

        const auto check_add_layer = [&layers_count, &output, &flags] (const DeviceLayer layer) noexcept
        {
            if (flags.test(layer))
            {
                assert(layers_count < output.size());
                output[layers_count] = to_string_view(layer).data();
                ++layers_count;
            }
        };

        check_add_layer(DeviceLayer::VkLayerKhronosValidation);

        assert(layers_count <= output.size());
        return {output.data(), layers_count};
    }
}
