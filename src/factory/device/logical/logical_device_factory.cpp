#include "logical_device_factory.h"
#include "device_requirements.h"
#include "factory_tools.hpp"

namespace ge::impl::factory::device::logical
{

    namespace impl
    {
        std::vector<std::string> get_available_device_layers(const vk::PhysicalDevice& device)
        {
            return ge::impl::factory::impl::layers_names(device.enumerateDeviceLayerProperties());
        }
    } // namespace impl

    vk::UniqueDevice create
    (
        const vk::PhysicalDevice& physical_device
      , uint32_t queue_family_index
    )
    {
        using namespace ge::impl::factory::impl;

        static constexpr const char* required_layers[] =
        {
            "VK_LAYER_LUNARG_standard_validation"
        };
        all_required_are_available(required_layers, impl::get_available_device_layers(physical_device));

        const auto queue_priority = 1.0f;
        const vk::DeviceQueueCreateInfo queue_create_info
        (
            {}
          , queue_family_index
          , 1
          , &queue_priority
        );

        vk::PhysicalDeviceFeatures device_features;
        device_features.setSamplerAnisotropy(VK_TRUE);

        const auto& required_extensions = get_required_device_extensions();

        vk::DeviceCreateInfo device_create_info
        (
            {}
          , 1
          , &queue_create_info
          , static_cast<uint32_t>(std::size(required_layers))
          , std::data(required_layers)
          , static_cast<uint32_t>(std::size(required_extensions))
          , std::data(required_extensions)
          , &device_features
        );

        return physical_device.createDeviceUnique(device_create_info);
    }

} // namespace ge::impl::factory::device::logical
