#pragma once

#include "exception.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::instance
{

    template<typename Extensions, typename Layers>
    vk::UniqueInstance create
    (
        const Extensions& required_extensions
      , const Layers& required_layers
    );

} // namespace ge::impl::factory::instance

#include "instance_factory_impl.hpp"
