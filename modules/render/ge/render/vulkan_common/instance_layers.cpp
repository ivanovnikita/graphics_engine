#include "instance_layers.h"

#include <vulkan/vulkan.h>

namespace ge
{
    std::string_view to_string_view(const InstanceLayer layer)
    {
        switch (layer)
        {
        case InstanceLayer::VkLayerKhronosValidation: return "VK_LAYER_KHRONOS_validation";
        }

        __builtin_unreachable();
    }

    std::span<const char*> get_required_layers
    (
        InstanceLayersStorage& output,
        const InstanceLayerFlags& flags
    ) noexcept
    {
        size_t layers_count = 0;

        const auto check_add_layer = [&layers_count, &output, &flags] (const InstanceLayer layer) noexcept
        {
            if (flags.test(layer))
            {
                assert(layers_count < output.size());
                output[layers_count] = to_string_view(layer).data();
                ++layers_count;
            }
        };

        check_add_layer(InstanceLayer::VkLayerKhronosValidation);

        assert(layers_count <= output.size());
        return {output.data(), layers_count};
    }
}
