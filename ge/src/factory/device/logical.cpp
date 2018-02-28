#include "factory/device/logical.h"
#include "factory/device/requirements.h"
#include "factory/device/tools.h"
#include "factory/tools.hpp"
#include "utils/safe_cast.hpp"

#include <set>

namespace ge::impl::factory::device::logical
{

    namespace
    {
        void emplace_if_has_value(std::set<uint32_t>& container, const std::optional<uint32_t>& optional)
        {
            if (optional.has_value())
            {
                container.emplace(optional.value());
            }
        }

        std::set<uint32_t> get_unique(const QueueFamilyIndices& queue_family_indeces)
        {
            std::set<uint32_t> unique_queue_family_indices;

            emplace_if_has_value(unique_queue_family_indices, queue_family_indeces.graphics);
            emplace_if_has_value(unique_queue_family_indices, queue_family_indeces.present);
            emplace_if_has_value(unique_queue_family_indices, queue_family_indeces.compute);
            emplace_if_has_value(unique_queue_family_indices, queue_family_indeces.transfer);

            return unique_queue_family_indices;
        }

        std::vector<vk::DeviceQueueCreateInfo> build_queue_create_info(const QueueFamilyIndices& queue_family_indeces)
        {
            const std::set<uint32_t> unique_queue_family_indices = get_unique(queue_family_indeces);

            const auto queue_priority = 0.0f;
            const auto queue_count = 1;
            std::vector<vk::DeviceQueueCreateInfo> queue_create_info;
            queue_create_info.reserve(unique_queue_family_indices.size());
            for (const auto& queue_family_index : unique_queue_family_indices)
            {
                queue_create_info.emplace_back(vk::DeviceQueueCreateFlags(), queue_family_index, queue_count, &queue_priority);
            }

            return queue_create_info;
        }
    } // unnamed namespace

    // TODO: compile time check: instance & physical device must enable validation layers
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

        const auto& required_extensions = get_required_device_extensions();

        const std::vector<vk::DeviceQueueCreateInfo> queue_create_infos = build_queue_create_info(queue_family_indeces);

        vk::PhysicalDeviceFeatures device_features;
        device_features.setSamplerAnisotropy(VK_TRUE);

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
