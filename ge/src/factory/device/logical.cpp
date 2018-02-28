#include "factory/device/logical.h"
#include "factory/device/requirements.h"
#include "factory/device/tools.h"
#include "factory/tools.hpp"
#include "utils/safe_cast.hpp"

#include <set>

namespace ge::impl::factory::device::logical
{

    vk::UniqueDevice create
    (
        const options::ValidationLayers& option_validation_layers
      , const vk::PhysicalDevice& physical_device
      , const QueueFamilyIndices& queue_family_indeces
    )
    {
        using namespace tools;

        const auto& required_layers = get_required_layers(option_validation_layers);
        all_required_are_available(required_layers, get_available_device_layers(physical_device));

        std::set<uint32_t> unique_queue_family_indices;
        if (queue_family_indeces.graphics.has_value())
        {
            unique_queue_family_indices.emplace(queue_family_indeces.graphics.value());
        }
        if (queue_family_indeces.present.has_value())
        {
            unique_queue_family_indices.emplace(queue_family_indeces.present.value());
        }
        if (queue_family_indeces.compute.has_value())
        {
            unique_queue_family_indices.emplace(queue_family_indeces.compute.value());
        }
        if (queue_family_indeces.transfer.has_value())
        {
            unique_queue_family_indices.emplace(queue_family_indeces.transfer.value());
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
          , safe_cast<uint32_t>(std::size(queue_create_infos))
          , std::data(queue_create_infos)
          , safe_cast<uint32_t>(std::size(required_layers))
          , std::data(required_layers)
          , safe_cast<uint32_t>(std::size(required_extensions))
          , std::data(required_extensions)
          , &device_features
        );

        return physical_device.createDeviceUnique(device_create_info);
    }

}
