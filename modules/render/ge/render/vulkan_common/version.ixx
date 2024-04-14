module;

#include <cstdint>

export module vulkan_common.version;

export import log;

namespace ge
{
    export struct Version final
    {
        static Version from_vulkan_version(uint32_t);
        uint32_t to_vulkan_version() const;

        uint32_t major;
        uint32_t minor;
        uint32_t patch;
        uint32_t variant;
    };

    export void log_non_trivial(LogDestination, const Version&) noexcept;
}
