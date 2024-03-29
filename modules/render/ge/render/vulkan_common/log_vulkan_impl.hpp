#pragma once

#include "log_vulkan.hpp"
#include "ge/common/bits_range.hpp"

#include "vulkan_refl.hpp"
#include "to_string_view_enum.h"

namespace ge
{
    template <typename T>
        requires ConvertibleToStringView<T>
    void log_non_trivial(const LogDestination destination, const vk::Flags<T>& flags) noexcept
    {
        const auto bits = static_cast<typename vk::Flags<T>::MaskType>(flags);
        log(destination, to_enum_bits_range<T>(bits));
    }

    template <typename T>
        requires (not ConvertibleToStringView<T>)
    void log_non_trivial(const LogDestination destination, const vk::Flags<T>&) noexcept
    {
        log(destination, "<bits are unknown>");
    }

    template <typename T>
        requires vk::isVulkanHandleType<T>::value
    void log_non_trivial(const LogDestination destination, const T&) noexcept
    {
        log(destination, "<handle>");
    }
}
