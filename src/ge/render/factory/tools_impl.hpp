#pragma once

#include "ge/render/factory/tools.hpp"
#include "ge/render/exception.h"

#include <vulkan/vulkan.hpp>

#include <string>
#include <algorithm>

namespace ge::factory
{
    template
    <
        std::ranges::input_range What
        , std::ranges::input_range Where
    >
        requires std::equality_comparable_with
        <
            std::ranges::range_value_t<What>
          , std::ranges::range_value_t<Where>
        >
    constexpr RangeView<std::ranges::range_value_t<What>> auto not_contained_in
    (
        const What& what
      , const Where& where
    ) noexcept
    {
        return what | std::views::filter
        (
            [&where] (const std::ranges::range_value_t<What>& required) noexcept
            {
                return std::ranges::none_of
                (
                    where
                    , [&required] (const std::ranges::range_value_t<Where>& available) noexcept
                    {
                        return required == available;
                    }
                );
            }
        );
    }

    template
    <
        std::ranges::input_range Required
      , std::ranges::input_range Available
    >
        requires std::equality_comparable_with
        <
            std::ranges::range_value_t<Required>
          , std::ranges::range_value_t<Available>
        >
    void all_required_are_available
    (
        const Required& required
      , const Available& available
    )
    {
        using RequiredValueType = std::ranges::range_value_t<Required>;

        RangeView<RequiredValueType> auto absent_values = not_contained_in
        (
            required
          , available
        );

        std::string error_message;
        bool any_absent = false;
        for (const RequiredValueType& name : absent_values)
        {
            if (not any_absent)
            {
                any_absent = true;
                error_message = "These features are not available: \n";
            }

            error_message.append(name);
            error_message.append("\n");
        }

        if (any_absent)
        {
            GE_THROW(device_capabilities_error, error_message);
        }
    }

    inline auto get_extension_name(const vk::ExtensionProperties& property)
    {
        return property.extensionName;
    }

    inline auto get_layer_name(const vk::LayerProperties& property)
    {
        return property.layerName;
    }

    template<typename Container, typename NameGetter>
    std::vector<std::string> properties_names(const Container& extensions, NameGetter get_name)
    {
        std::vector<std::string> result;
        result.reserve(extensions.size());
        for (const auto& extension : extensions)
        {
            result.emplace_back(get_name(extension));
        }
        return result;
    }

    template<typename Container>
    std::vector<std::string> extensions_names(const Container& extensions)
    {
        return properties_names(extensions, get_extension_name);
    }

    template<typename Container>
    std::vector<std::string> layers_names(const Container& layers)
    {
        return properties_names(layers, get_layer_name);
    }
}
