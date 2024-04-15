module;

#include "vulkan_fwds.h"

export module vulkan_common.command_pool;

export import vulkan_common.device;

namespace ge
{
    export vk::UniqueCommandPool create_command_pool(const DeviceData&);
}
