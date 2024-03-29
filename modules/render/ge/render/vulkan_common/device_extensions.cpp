#include "device_extensions.h"

#include <vulkan/vulkan.h>

namespace ge
{
    std::string_view to_string_view(const DeviceExtension extension)
    {
        switch (extension)
        {
        case DeviceExtension::VkKhrSwapchain: return VK_KHR_SWAPCHAIN_EXTENSION_NAME;
        }

        __builtin_unreachable();
    }

    std::span<const char*> get_required_extensions
    (
        DeviceExtensionStorage& output,
        const DeviceExtensionFlags& flags
    ) noexcept
    {
        size_t extensions_count = 0;

        const auto check_add_extension =
        [&extensions_count, &output, &flags] (const DeviceExtension extension) noexcept
        {
            if (flags.test(extension))
            {
                assert(extensions_count < output.size());
                output[extensions_count] = to_string_view(extension).data();
                ++extensions_count;
            }
        };

        check_add_extension(DeviceExtension::VkKhrSwapchain);

        assert(extensions_count <= output.size());
        return {output.data(), extensions_count};
    }
}
