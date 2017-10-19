#pragma once

#include "logical_device_factory.hpp"
#include "factory_tools.hpp"
#include "device_tools.h"

namespace ge::impl::factory::device::logical
{

    namespace impl
    {
        std::vector<std::string> get_available_device_layers(const vk::PhysicalDevice&);
    } // namespace impl

    template<typename Layers>
    vk::UniqueDevice create
    (
        const vk::PhysicalDevice& physical_device
      , const Layers& required_layers
      , const vk::SurfaceKHR& surface
    )
    {
        using namespace ge::impl::factory::impl;
        using LayersType = std::remove_pointer_t<decltype(std::data(required_layers))>;

        static_assert(std::is_same_v<std::remove_const_t<std::remove_pointer_t<LayersType>>, char>
                      && std::is_pointer_v<LayersType>, "Layers names must be saved as char*");

        all_required_are_available(required_layers, impl::get_available_device_layers(physical_device));

        const auto queue_priority = 1.0f;
        const vk::DeviceQueueCreateInfo queue_create_info
        (
            {}
          , static_cast<uint32_t>(get_suitable_queue_family_index(physical_device, surface))
          , 1
          , &queue_priority
        );

        vk::PhysicalDeviceFeatures device_features;
        device_features.setSamplerAnisotropy(true);

        const auto& required_extensions = get_required_device_extensions();

        vk::DeviceCreateInfo device_create_info
        (
            {}
          , 1
          , &queue_create_info
          , std::size(required_layers)
          , std::data(required_layers)
          , std::size(required_extensions)
          , std::data(required_extensions)
          , &device_features
        );

        return physical_device.createDeviceUnique(device_create_info);
    }

} // namespace ge::impl::factory::device::physical
