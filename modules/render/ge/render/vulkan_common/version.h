#pragma once

#include "ge/common/log.hpp"

#include <cstdint>

namespace ge
{
    struct Version final
    {
        static Version from_vulkan_version(uint32_t);
        uint32_t to_vulkan_version() const;

        uint32_t major;
        uint32_t minor;
        uint32_t patch;
        uint32_t variant;
    };

    void log_non_trivial(LogDestination, const Version&) noexcept;
}
