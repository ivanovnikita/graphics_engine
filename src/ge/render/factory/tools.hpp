#pragma once

#include "ge/render/exception.h"
#include "ge/render/factory/options.h"

#include <vector>
#include <ranges>
#include <concepts>

namespace ge::factory
{
    template
    <
        typename RangeType
        , typename ValueType = std::ranges::range_value_t<RangeType>
    >
    concept RangeView = std::ranges::view<RangeType>;

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
    ) noexcept;

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
    ) noexcept;

    template<typename Container>
    std::vector<std::string> extensions_names(const Container& extensions);

    template<typename Container>
    std::vector<std::string> layers_names(const Container& layers);

    std::vector<const char*> get_required_layers(const options::ValidationLayers&);
}

#include "ge/render/factory/tools_impl.hpp"
