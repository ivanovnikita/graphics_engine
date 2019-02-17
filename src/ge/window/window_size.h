#pragma once

#include <variant>
#include <optional>

namespace ge
{
    struct Size final
    {
        uint16_t width;
        uint16_t height;
    };

    using StaticSize = Size;

    struct DynamicSize final
    {
        Size default_size;
        std::optional<Size> min_size;
        std::optional<Size> max_size;
    };

    using WindowSize = std::variant<StaticSize, DynamicSize>;
}
