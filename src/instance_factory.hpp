#pragma once

#include "exception.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::instance_factory
{

    template<typename Extensions, typename Layers>
    vk::UniqueInstance create
    (
        const Extensions& required_extensions
      , const Layers& required_layers
    );

} // namespace ge::impl::instance_factory

#include "instance_factory_impl.hpp"
