#pragma once

#include <glm/glm.hpp>

#include <variant>
#include <span>
#include <vector>

namespace ge
{
    struct Vertex final
    {
        glm::vec2 pos;
    };

    struct Color final
    {
        glm::vec3 color;
    };

    template <typename T>
    struct Required final
    {
        T value;
    };

    template <typename T>
    struct Optional final
    {
        T value;
    };

    template <typename T>
    using Param = std::variant<Required<T>, Optional<T>>;
}
