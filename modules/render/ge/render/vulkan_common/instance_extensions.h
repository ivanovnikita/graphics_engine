#pragma once

#include "ge/common/flags.hpp"

#include <array>
#include <span>
#include <string_view>

namespace ge
{
    enum class InstanceExtension
    {
        VkExtDebugReport = 1,
        VkKhrSurface = 1 << 1,

        // TODO: add define for xcb/linux
        VkKhrXcbSurface = 1 << 2,

#ifdef _WIN32
        VkKhrWin32Surface = 1 << 3,
#endif
    };

    std::string_view to_string_view(InstanceExtension);

    constexpr size_t INSTANCE_EXTENSION_MAX_COUNT = 4;
    using InstanceExtensionStorage = std::array<const char*, INSTANCE_EXTENSION_MAX_COUNT>;

    using InstanceExtensionFlags = Flags<InstanceExtension>;

    std::span<const char*> get_required_extensions
    (
        InstanceExtensionStorage& output,
        const InstanceExtensionFlags&
    ) noexcept;
}
