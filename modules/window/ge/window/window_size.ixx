module;

#include <variant>
#include <optional>

#include <cstdint>

export module window_size;

namespace ge
{
    export struct Size final
    {
        uint16_t width;
        uint16_t height;
    };

    export using StaticSize = Size;

    export struct DynamicSize final
    {
        Size default_size;
        std::optional<Size> min_size;
        std::optional<Size> max_size;
    };

    export using WindowSize = std::variant<StaticSize, DynamicSize>;
}
