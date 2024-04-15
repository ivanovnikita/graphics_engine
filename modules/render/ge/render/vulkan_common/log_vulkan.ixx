module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.log_vulkan;

export import log;
export import bits_range;
export import vulkan_refl;
export import to_string_view_enum;

namespace ge
{
    export template <typename T>
        requires ConvertibleToStringView<T>
    void log_non_trivial(LogDestination, const vk::Flags<T>&) noexcept;

    export template <typename T>
        requires (not ConvertibleToStringView<T>)
    void log_non_trivial(LogDestination, const vk::Flags<T>&) noexcept;

    export template <typename T>
        requires vk::isVulkanHandleType<T>::value
    void log_non_trivial(LogDestination, const T&) noexcept;
}

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
