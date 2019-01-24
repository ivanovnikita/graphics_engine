#include "factory/swapchain.h"
#include "utils/safe_cast.hpp"

namespace ge::factory
{
    namespace
    {
        vk::SurfaceFormatKHR choose_format(const vk::PhysicalDevice& physical_device, const vk::SurfaceKHR& surface)
        {
            const auto surface_formats = physical_device.getSurfaceFormatsKHR(surface);

            if (surface_formats.size() == 1 && surface_formats[0].format == vk::Format::eUndefined)
            {
                vk::SurfaceFormatKHR format;
                format.format = vk::Format::eB8G8R8A8Unorm;
                format.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
                return format;
            }

            for (const auto& format : surface_formats)
            {
                if (format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                {
                    return format;
                }
            }

            return surface_formats.at(0);
        }

        vk::PresentModeKHR choose_present_mode(const vk::PhysicalDevice& physical_device, const vk::SurfaceKHR& surface)
        {
            const auto surface_present_modes = physical_device.getSurfacePresentModesKHR(surface);

            vk::PresentModeKHR best = vk::PresentModeKHR::eFifo;

            for (const auto& mode : surface_present_modes)
            {
                if (mode == vk::PresentModeKHR::eMailbox)
                {
                    return mode;
                }
                else if (mode == vk::PresentModeKHR::eImmediate)
                {
                    best = mode;
                }
            }

            return best;
        }

        vk::Extent2D choose_extent(const vk::SurfaceCapabilitiesKHR& capabilities, const Window& window)
        {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            {
                return capabilities.currentExtent;
            }
            else
            {
                const auto[width, height] = window.extent();
                return
                {
                    std::max
                    (
                        capabilities.minImageExtent.width
                      , std::min(capabilities.maxImageExtent.width, safe_cast<uint32_t>(width))
                    )
                  , std::max
                    (
                        capabilities.minImageExtent.height
                      , std::min(capabilities.maxImageExtent.height, safe_cast<uint32_t>(height))
                    )
                };
            }
        }

        uint32_t choose_image_count(const vk::SurfaceCapabilitiesKHR& surface_capabilities)
        {
            uint32_t count = surface_capabilities.minImageCount + 1;
            if (surface_capabilities.maxImageCount != 0)
            {
                count = std::min(count, surface_capabilities.maxImageCount);
            }
            return count;
        }

        std::pair<vk::SharingMode, std::vector<uint32_t>> choose_sharing_mode_and_indices
        (
            const QueueFamilyIndices& queue_family_indices
        )
        {
            assert(queue_family_indices.graphics.has_value());
            assert(queue_family_indices.present.has_value());

            if (queue_family_indices.graphics == queue_family_indices.present)
            {
                return
                {
                    vk::SharingMode::eExclusive
                  , std::vector<uint32_t>()
                };
            }
            else
            {
                return
                {
                    vk::SharingMode::eConcurrent
                  , {queue_family_indices.graphics.value(), queue_family_indices.present.value()}
                };
            }
        }
    }

    std::pair<vk::UniqueSwapchainKHR, vk::Format> create_swapchain
    (
        const vk::PhysicalDevice& physical_device
      , const vk::Device& logical_device
      , const Window& window
      , const vk::SurfaceKHR& surface
      , const QueueFamilyIndices& queue_family_indices
    )
    {
        const auto surface_capabilities = physical_device.getSurfaceCapabilitiesKHR(surface);

        const uint32_t image_count = choose_image_count(surface_capabilities);

        const auto format = choose_format(physical_device, surface);
        const auto[sharing_mode, indices] = choose_sharing_mode_and_indices(queue_family_indices);

        const uint32_t array_layers_count = 1;
        const auto is_clipped = VK_TRUE;

        const vk::SwapchainCreateInfoKHR create_info
        {
            vk::SwapchainCreateFlagsKHR{}
          , surface
          , image_count
          , format.format
          , format.colorSpace
          , choose_extent(surface_capabilities, window)
          , array_layers_count
          , vk::ImageUsageFlagBits::eColorAttachment
          , sharing_mode
          , safe_cast<uint32_t>(std::size(indices))
          , std::data(indices)
          , surface_capabilities.currentTransform
          , vk::CompositeAlphaFlagBitsKHR::eOpaque
          , choose_present_mode(physical_device, surface)
          , is_clipped
        };

        return {logical_device.createSwapchainKHRUnique(create_info), format.format};
    }
}
