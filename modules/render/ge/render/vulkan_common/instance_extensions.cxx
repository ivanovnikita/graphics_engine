module;

#include <vulkan/vulkan.h>

#include <span>
#include <string_view>

#include <cassert>

module vulkan_common.instance_extensions;

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
