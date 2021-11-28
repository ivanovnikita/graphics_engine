#pragma once

#include <functional>
#include <array>

#include <cstdint>

namespace vk
{
    class Instance;
    class SurfaceKHR;

    template <typename Type, typename Dispatch>
    class UniqueHandle;

    class DispatchLoaderStatic;

    using UniqueSurfaceKHR = UniqueHandle<SurfaceKHR, DispatchLoaderStatic>;
}

namespace ge
{
    using SurfaceCreator = vk::UniqueSurfaceKHR(const vk::Instance&);

    struct SurfaceParams final
    {
        std::function<SurfaceCreator> surface_creator;
        uint32_t width;
        uint32_t height;
        std::array<uint8_t, 4> background_color;
    };
}
