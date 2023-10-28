#include "device_layers.h"

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
