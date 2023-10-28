#include "version.h"

#include <vulkan/vulkan.h>

namespace ge
{
    Version Version::from_vulkan_version(const uint32_t version)
    {
        return Version
        {
            .major = VK_API_VERSION_MAJOR(version),
            .minor = VK_API_VERSION_MINOR(version),
            .patch = VK_API_VERSION_PATCH(version),
            .variant = VK_API_VERSION_VARIANT(version),
        };
    }

    uint32_t Version::to_vulkan_version() const
    {
        return VK_MAKE_API_VERSION(variant, major, minor, patch);
    }

    void log_non_trivial(const LogDestination destination, const Version& version) noexcept
    {
        log(destination, version.major, ".", version.minor, ".", version.patch, "-", version.variant);
    }
}

