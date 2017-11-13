#pragma once

#include "options.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::instance
{

    vk::UniqueInstance create(const OptionsInstance&);

} // namespace ge::impl::factory::instance
