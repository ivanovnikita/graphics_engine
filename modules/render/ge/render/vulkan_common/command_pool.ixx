export module vulkan_common.command_pool;

import vulkan_common.vulkan_fwds;
import vulkan_common.device_fwd;

namespace ge
{
    export vk::UniqueCommandPool create_command_pool(const DeviceData&);
}
