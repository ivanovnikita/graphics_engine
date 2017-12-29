#include "swapchain_factory.h"

namespace ge::impl::factory::swapchain
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
                      , std::min(capabilities.maxImageExtent.width, static_cast<uint32_t>(width))
                    )
                  , std::max
                    (
                        capabilities.minImageExtent.height
                      , std::min(capabilities.maxImageExtent.height, static_cast<uint32_t>(height))
                    )
                };
            }
        }

    } // unnamed namespace

    std::pair<vk::UniqueSwapchainKHR, vk::Format> create
    (
        const vk::PhysicalDevice& physical_device
      , const vk::Device& logical_device
      , const Window& window
      , const vk::SurfaceKHR& surface
      , const QueueFamilyIndices& queue_family_indices
    )
    {
        const auto surface_capabilities = physical_device.getSurfaceCapabilitiesKHR(surface);

        const uint32_t image_count = [&surface_capabilities]
        {
            uint32_t count = surface_capabilities.minImageCount + 1;
            if (surface_capabilities.maxImageCount != 0)
            {
                count = std::min(count, surface_capabilities.maxImageCount);
            }
            return count;
        }();

        const auto format = choose_format(physical_device, surface);
        const auto[sharing_mode, indices] = [&queue_family_indices]
        {
            if (queue_family_indices.graphics == queue_family_indices.present)
            {
                return std::pair<vk::SharingMode, std::vector<uint32_t>>
                {
                    vk::SharingMode::eExclusive
                  , std::vector<uint32_t>()
                };
            }
            else
            {
                return std::pair<vk::SharingMode, std::vector<uint32_t>>
                {
                    vk::SharingMode::eConcurrent
                  , std::vector<uint32_t>{queue_family_indices.graphics, queue_family_indices.present}
                };
            }
        }();

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
          , static_cast<uint32_t>(std::size(indices))
          , std::data(indices)
          , surface_capabilities.currentTransform
          , vk::CompositeAlphaFlagBitsKHR::eOpaque
          , choose_present_mode(physical_device, surface)
          , is_clipped
        };

        return {logical_device.createSwapchainKHRUnique(create_info), format.format};
    }

} // namespace ge::impl::factory::swapchain
