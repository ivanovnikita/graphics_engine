module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.instance;

export import vulkan_common.version;
export import vulkan_common.instance_layers;
export import vulkan_common.instance_extensions;
export import logger;

namespace ge
{
    export struct InstanceData final
    {
        static InstanceData create_default
        (
            InstanceLayerFlags,
            InstanceExtensionFlags,
            const Logger&
        );

        vk::UniqueInstance instance;
        Version api_version;
        InstanceLayerFlags enabled_layers;
        InstanceExtensionFlags enabled_extension;
        vk::UniqueDebugReportCallbackEXT debug_callback;
    };
}
