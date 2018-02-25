#pragma once

#include "factory/options.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::instance
{

    vk::UniqueInstance create(const OptionsInstance&);

}
