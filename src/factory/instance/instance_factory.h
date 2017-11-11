#pragma once

#include "options.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::instance
{

    struct OptionsInstance final
    {
        OptionsDebug debug;
        OptionWindow window;
    };

    vk::UniqueInstance create(const OptionsInstance&);

} // namespace ge::impl::factory::instance
