module;

#include <array>
#include <span>
#include <string_view>

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
