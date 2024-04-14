module;

#include <vulkan/vulkan.h>

#include <array>
#include <span>
#include <string_view>

#include <cassert>

export module vulkan_common.instance_extensions;

export import flags;

namespace ge
{
    export enum class InstanceExtension
    {
        VkExtDebugReport = 1,
        VkKhrSurface = 1 << 1,

        // TODO: add define for xcb/linux
        VkKhrXcbSurface = 1 << 2,

#ifdef _WIN32
        VkKhrWin32Surface = 1 << 3,
#endif
    };

    export std::string_view to_string_view(InstanceExtension);

    static constexpr size_t INSTANCE_EXTENSION_MAX_COUNT = 4;
    export using InstanceExtensionStorage = std::array<const char*, INSTANCE_EXTENSION_MAX_COUNT>;

    export using InstanceExtensionFlags = Flags<InstanceExtension>;

    export std::span<const char*> get_required_extensions
    (
        InstanceExtensionStorage& output,
        const InstanceExtensionFlags&
    ) noexcept;
}

module : private;

namespace ge
{
    std::string_view to_string_view(const InstanceExtension extension)
    {
        switch (extension)
        {
        case InstanceExtension::VkExtDebugReport: return VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
        case InstanceExtension::VkKhrSurface: return VK_KHR_SURFACE_EXTENSION_NAME;
        case InstanceExtension::VkKhrXcbSurface: return VK_KHR_XCB_SURFACE_EXTENSION_NAME;
#ifdef _WIN32
        case InstanceExtension::VkKhrWin32Surface: return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#endif
        }

        __builtin_unreachable();
    }

    std::span<const char*> get_required_extensions
    (
        InstanceExtensionStorage& output,
        const InstanceExtensionFlags& flags
    ) noexcept
    {
        size_t extensions_count = 0;

        const auto check_add_extension =
        [&extensions_count, &output, &flags] (const InstanceExtension extension) noexcept
        {
            if (flags.test(extension))
            {
                assert(extensions_count < output.size());
                output[extensions_count] = to_string_view(extension).data();
                ++extensions_count;
            }
        };

        check_add_extension(InstanceExtension::VkExtDebugReport);
        check_add_extension(InstanceExtension::VkKhrSurface);
        check_add_extension(InstanceExtension::VkKhrXcbSurface);
#ifdef _WIN32
        check_add_extension(InstanceExtension::VkKhrWin32Surface);
#endif

        assert(extensions_count <= output.size());
        return {output.data(), extensions_count};
    }
}
