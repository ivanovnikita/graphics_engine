#include "ge/common/safe_cast.hpp"
#include "ge/render/factory/device/logical.h"
#include "ge/render/factory/device/requirements.h"
#include "ge/render/factory/device/tools.h"
#include "ge/render/factory/tools.hpp"

#include <set>

namespace ge::factory
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

            constexpr float queue_priority = 0.0f;

            auto queue_create_info = vk::DeviceQueueCreateInfo{}
                .setQueueCount(1)
                .setPQueuePriorities(&queue_priority);

            std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
            queue_create_infos.reserve(unique_queue_family_indices.size());

            for (const auto& queue_family_index : unique_queue_family_indices)
            {
                queue_create_info.setQueueFamilyIndex(queue_family_index);
                queue_create_infos.emplace_back(queue_create_info);
            }

            return queue_create_infos;
        }
    }

    // TODO: compile time check: instance & physical device must enable validation layers
    vk::UniqueDevice create_logical_device
    (
        const options::ValidationLayers& option_validation_layers
      , const vk::PhysicalDevice& physical_device
      , const QueueFamilyIndices& queue_family_indeces
    )
    {
        const auto& required_layers = get_required_layers(option_validation_layers);
        all_required_are_available(required_layers, get_available_device_layers(physical_device));

        const auto& required_extensions = get_required_device_extensions();

        const std::vector<vk::DeviceQueueCreateInfo> queue_create_infos = build_queue_create_info(queue_family_indeces);

        vk::PhysicalDeviceFeatures device_features;
        device_features.setSamplerAnisotropy(VK_TRUE);

        device_features.setFillModeNonSolid(VK_TRUE);
        device_features.setWideLines(VK_TRUE);

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
