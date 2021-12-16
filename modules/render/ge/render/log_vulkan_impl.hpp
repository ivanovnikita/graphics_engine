#pragma once

#include "log_vulkan.hpp"
#include "ge/common/bits_range.hpp"

namespace ge
{
    template <typename T>
        requires ConvertibleToStringView<T>
    void log_non_trivial(const LogDestination destination, const vk::Flags<T>& flags) noexcept
    {
        const auto bits = static_cast<typename vk::Flags<T>::MaskType>(flags);
        log(destination, to_enum_bits_range<T>(bits));
    }
}
