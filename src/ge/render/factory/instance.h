#pragma once

#include "ge/render/factory/options.h"

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    vk::UniqueInstance create_instance(const options::Instance&);
}
