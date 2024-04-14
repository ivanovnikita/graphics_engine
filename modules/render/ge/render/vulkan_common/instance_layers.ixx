module;

#include <vulkan/vulkan.h>

#include <array>
#include <span>
#include <string_view>

#include <cassert>

export module vulkan_common.instance_layers;

export import flags;

namespace ge
{
    export enum class InstanceLayer
    {
        VkLayerKhronosValidation = 1
    };

    export std::string_view to_string_view(InstanceLayer);

    static constexpr size_t INSTANCE_LAYER_MAX_COUNT = 1;
    export using InstanceLayersStorage = std::array<const char*, INSTANCE_LAYER_MAX_COUNT>;

    export using InstanceLayerFlags = Flags<InstanceLayer>;

    export std::span<const char*> get_required_layers
    (
        InstanceLayersStorage& output,
        const InstanceLayerFlags&
    ) noexcept;
}

module : private;

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
