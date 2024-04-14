module;

#include <array>
#include <span>
#include <string_view>

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
