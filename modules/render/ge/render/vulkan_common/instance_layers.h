#pragma once

#include "ge/common/flags.hpp"

#include <array>
#include <span>
#include <string_view>

namespace ge
{
    enum class InstanceLayer
    {
        VkLayerKhronosValidation = 1
    };

    std::string_view to_string_view(InstanceLayer);

    constexpr size_t INSTANCE_LAYER_MAX_COUNT = 1;
    using InstanceLayersStorage = std::array<const char*, INSTANCE_LAYER_MAX_COUNT>;

    using InstanceLayerFlags = Flags<InstanceLayer>;

    std::span<const char*> get_required_layers
    (
        InstanceLayersStorage& output,
        const InstanceLayerFlags&
    ) noexcept;
}
