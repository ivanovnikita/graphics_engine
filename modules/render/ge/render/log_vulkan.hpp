#pragma once

#include "ge/common/log.hpp"

#include <vulkan/vulkan.hpp>

namespace ge
{
    template <typename T>
        requires ConvertibleToStringView<T>
    void log_non_trivial(LogDestination, const vk::Flags<T>&) noexcept;
}

#include "log_vulkan_impl.hpp"
