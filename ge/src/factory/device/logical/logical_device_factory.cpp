#include "logical_device_factory.h"
#include "device_requirements.h"
#include "factory_tools.hpp"

#include <set>

namespace ge::impl::factory::device::logical
{

    namespace
    {
        std::vector<std::string> get_available_device_layers(const vk::PhysicalDevice& device)
        {
            return ge::impl::factory::impl::layers_names(device.enumerateDeviceLayerProperties());
        }
    } // unnamed namespace

    vk::UniqueDevice create
    (
        const OptionsDevice& options
      , const vk::PhysicalDevice& physical_device
      , QueueFamilyIndices queue_family_indeces
    )
    {
        using namespace ge::impl::factory::impl;

        static constexpr const char* required_layers[] =
        {
            "VK_LAYER_LUNARG_standard_validation"
        };
        all_required_are_available(required_layers, get_available_device_layers(physical_device));

        std::set<uint32_t> unique_queue_family_indices;
        if (options.graphics.enabled)
        {
            unique_queue_family_indices.emplace(queue_family_indeces.graphics);
            unique_queue_family_indices.emplace(queue_family_indeces.present);
        }

        if (options.compute.enabled)
        {
            unique_queue_family_indices.emplace(queue_family_indeces.compute);
        }

        if (options.transfer.enabled)
        {
            unique_queue_family_indices.emplace(queue_family_indeces.transfer);
        }

        const auto queue_priority = 0.0f;
        const auto queue_count = 1;
        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
        queue_create_infos.reserve(unique_queue_family_indices.size());
        for (const auto& queue_family_index : unique_queue_family_indices)
        {
            queue_create_infos.emplace_back(vk::DeviceQueueCreateFlags(), queue_family_index, queue_count, &queue_priority);
        }

        vk::PhysicalDeviceFeatures device_features;
        device_features.setSamplerAnisotropy(VK_TRUE);

        const auto& required_extensions = get_required_device_extensions();

        vk::DeviceCreateInfo device_create_info
        (
            {}
          , static_cast<uint32_t>(std::size(queue_create_infos))
          , std::data(queue_create_infos)
          , static_cast<uint32_t>(std::size(required_layers))
          , std::data(required_layers)
          , static_cast<uint32_t>(std::size(required_extensions))
          , std::data(required_extensions)
          , &device_features
        );

        return physical_device.createDeviceUnique(device_create_info);
    }

} // namespace ge::impl::factory::device::logical
