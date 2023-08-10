#pragma once

#include "device.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::UniqueCommandPool create_command_pool(const DeviceData&);
}
