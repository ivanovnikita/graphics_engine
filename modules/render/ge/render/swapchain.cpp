#include "swapchain.h"
#include "exception.h"

#include <array>
#include <limits>

namespace ge
{
    namespace
    {
        vk::SurfaceCapabilitiesKHR get_surface_capabilities
        (
            const vk::PhysicalDevice& physical_device,
            const vk::SurfaceKHR& surface
        )
        {
            vk::SurfaceCapabilitiesKHR capabilities;

            const vk::Result result = physical_device.getSurfaceCapabilitiesKHR
            (
                surface,
                &capabilities
            );

            switch (result)
            {
            case vk::Result::eSuccess:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorSurfaceLostKHR:
                GE_THROW_EXPECTED_RESULT(result, "Surface capabilities requesting failed");
            default:
                GE_THROW_UNEXPECTED_RESULT(result, "Surface capabilities requesting failed");
            }

            return capabilities;
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

        vk::SurfaceFormatKHR choose_format
        (
            const vk::PhysicalDevice& physical_device,
            const vk::SurfaceKHR& surface
        )
        {
            uint32_t formats_count = 0;
            const vk::Result formats_count_enumeration_result = physical_device.getSurfaceFormatsKHR
            (
                surface,
                &formats_count,
                nullptr
            );
            switch (formats_count_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorSurfaceLostKHR:
                GE_THROW_EXPECTED_RESULT(formats_count_enumeration_result, "Counting surface formats failed");
            default:
                GE_THROW_UNEXPECTED_RESULT(formats_count_enumeration_result, "Counting surface formats failed");
            }

            if (formats_count == 0)
            {
                GE_THROW_EXPECTED_ERROR("Surface formats are absent");
            }

            std::vector<vk::SurfaceFormatKHR> formats;
            try
            {
                formats.reserve(formats_count);
            }
            catch (const std::bad_alloc&)
            {
                GE_THROW_EXPECTED_ERROR("Allocation for surface formats failed");
            }

            const vk::Result formats_enumeration_result = physical_device.getSurfaceFormatsKHR
            (
                surface,
                &formats_count,
                formats.data()
            );
            switch (formats_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorSurfaceLostKHR:
                GE_THROW_EXPECTED_RESULT(formats_enumeration_result, "Enumerating surface formats failed");
            default:
                GE_THROW_UNEXPECTED_RESULT(formats_enumeration_result, "Enumerating surface formats failed");
            }

            if (formats.size() == 1 and formats.front().format == vk::Format::eUndefined)
            {
                vk::SurfaceFormatKHR format;
                format.format = vk::Format::eB8G8R8A8Unorm;
                format.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
                return format;
            }

            for (const vk::SurfaceFormatKHR& format : formats)
            {
                if
                (
                    format.format == vk::Format::eB8G8R8A8Unorm
                    and format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear
                )
                {
                    return format;
                }
            }

            return formats.front();
        }

        vk::Extent2D choose_extent
        (
            const vk::SurfaceCapabilitiesKHR& capabilities,
            const vk::Extent2D& extent
        )
        {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            {
                return capabilities.currentExtent;
            }
            else
            {
                return vk::Extent2D
                {
                    std::max
                    (
                        capabilities.minImageExtent.width,
                        std::min(capabilities.maxImageExtent.width, extent.width)
                    ),
                    std::max
                    (
                        capabilities.minImageExtent.height,
                        std::min(capabilities.maxImageExtent.height, extent.height)
                    )
                };
            }
        }

        std::pair<vk::SharingMode, std::span<uint32_t>> choose_sharing_mode_and_indices
        (
            const DeviceData& device_data,
            std::span<uint32_t> output
        )
        {
            if (device_data.graphics_queue_family_index == device_data.present_queue_family_index)
            {
                return
                {
                    vk::SharingMode::eExclusive,
                    {output.data(), 0}
                };
            }

            assert(output.size() >= 2);

            output[0] = device_data.graphics_queue_family_index;
            output[1] = device_data.present_queue_family_index;

            return
            {
                vk::SharingMode::eConcurrent,
                {output.data(), 2}
            };
        }

        vk::PresentModeKHR choose_present_mode
        (
            const vk::PhysicalDevice& physical_device,
            const vk::SurfaceKHR& surface
        )
        {
            uint32_t present_modes_count = 0;
            const vk::Result present_modes_enumeration_count_result = physical_device.getSurfacePresentModesKHR
            (
                surface,
                &present_modes_count,
                nullptr
            );
            switch (present_modes_enumeration_count_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorSurfaceLostKHR:
                GE_THROW_EXPECTED_RESULT(present_modes_enumeration_count_result, "Counting surface presentation modes failed");
            default:
                GE_THROW_UNEXPECTED_RESULT(present_modes_enumeration_count_result, "Counting surface presentation modes failed");
            }

            if (present_modes_count == 0)
            {
                GE_THROW_EXPECTED_ERROR("Surface presentation modes are absent");
            }

            std::vector<vk::PresentModeKHR> present_modes;
            try
            {
                present_modes.reserve(present_modes_count);
            }
            catch(const std::bad_alloc&)
            {
                GE_THROW_EXPECTED_ERROR("Allocation for surface presentation modes failed");
            }

            const vk::Result present_modes_enumeration_result = physical_device.getSurfacePresentModesKHR
            (
                surface,
                &present_modes_count,
                present_modes.data()
            );
            switch (present_modes_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorSurfaceLostKHR:
                GE_THROW_EXPECTED_RESULT(present_modes_enumeration_result, "Enumeration surface presentation modes failed");
            default:
                GE_THROW_UNEXPECTED_RESULT(present_modes_enumeration_result, "Enumeration surface presentation modes failed");
            }

            vk::PresentModeKHR best = vk::PresentModeKHR::eFifo;

            for (const vk::PresentModeKHR& present_mode : present_modes)
            {
                if (present_mode == vk::PresentModeKHR::eMailbox)
                {
                    return present_mode;
                }
                else if (present_mode == vk::PresentModeKHR::eImmediate)
                {
                    best = present_mode;
                }
            }

            return best;
        }

        vk::UniqueSwapchainKHR create_swapchain
        (
            const vk::SwapchainCreateInfoKHR& create_info,
            const vk::Device& device
        )
        {
            vk::SwapchainKHR swapchain;

            const vk::Result create_result = device.createSwapchainKHR
            (
                &create_info,
                nullptr,
                &swapchain
            );
            switch (create_result)
            {
            case vk::Result::eSuccess:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorDeviceLost:
            case vk::Result::eErrorSurfaceLostKHR:
            case vk::Result::eErrorNativeWindowInUseKHR:
            case vk::Result::eErrorInitializationFailed:
            case vk::Result::eErrorCompressionExhaustedEXT:
                GE_THROW_EXPECTED_RESULT(create_result, "Swapchain creation failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(create_result, "Swapchain creation failed");
            }
            }

            return vk::UniqueSwapchainKHR
            {
                std::move(swapchain),
                vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
            };
        }

        std::vector<vk::Image> get_swapchain_images
        (
            const vk::Device& device,
            const vk::SwapchainKHR& swapchain
        )
        {
            uint32_t images_count = 0;
            const vk::Result images_enumeration_count_result = device.getSwapchainImagesKHR
            (
                swapchain,
                &images_count,
                nullptr
            );
            switch (images_enumeration_count_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
                GE_THROW_EXPECTED_RESULT(images_enumeration_count_result, "Counting images in swapchain failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(images_enumeration_count_result, "Counting images in swapchain failed");
            }
            }

            if (images_count == 0)
            {
                GE_THROW_EXPECTED_ERROR("Swapchain images are absent");
            }

            std::vector<vk::Image> images;
            try
            {
                images.reserve(images_count);
            }
            catch(const std::bad_alloc&)
            {
                GE_THROW_EXPECTED_ERROR("Allocation for swapchain images failed");
            }

            const vk::Result images_enumeration_result = device.getSwapchainImagesKHR
            (
                swapchain,
                &images_count,
                images.data()
            );
            switch (images_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
                GE_THROW_EXPECTED_RESULT(images_enumeration_result, "Enumeration images in swapchain failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(images_enumeration_result, "Enumeration images in swapchain failed");
            }
            }

            return images;
        }

        std::vector<vk::UniqueImageView> create_image_views
        (
            const std::vector<vk::Image>& images,
            const vk::Format format,
            const vk::Device& device
        )
        {
            std::vector<vk::UniqueImageView> views;
            try
            {
                views.reserve(images.size());
            }
            catch(const std::bad_alloc&)
            {
                GE_THROW_EXPECTED_ERROR("Allocation for swapchain image views failed");
            }

            const vk::ImageSubresourceRange subresource_range = vk::ImageSubresourceRange{}
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

            vk::ImageViewCreateInfo create_info = vk::ImageViewCreateInfo{}
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(format)
                .setSubresourceRange(subresource_range);

            for (const vk::Image& image : images)
            {
                create_info.setImage(image);

                vk::ImageView view;
                const vk::Result result = device.createImageView
                (
                    &create_info,
                    nullptr,
                    &view
                );

                switch (result)
                {
                case vk::Result::eSuccess:
                    break;
                case vk::Result::eErrorOutOfHostMemory:
                case vk::Result::eErrorOutOfDeviceMemory:
                case vk::Result::eErrorInvalidOpaqueCaptureAddressKHR:
                    GE_THROW_EXPECTED_RESULT(result, "Swapchain image view creation failed");
                default:
                {
                    GE_THROW_UNEXPECTED_RESULT(result, "Swapchain image view creation failed");
                }
                }

                vk::UniqueImageView unique_view
                {
                    std::move(view),
                    vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
                };
                views.emplace_back(std::move(unique_view));
            }

            return views;
        }
    }

    SwapchainData SwapchainData::create_default
    (
        const DeviceData& device_data,
        SurfaceData& surface_data
    )
    {
        const vk::SurfaceCapabilitiesKHR surface_capabilities = get_surface_capabilities
        (
            device_data.physical_device_data.physical_device,
            *surface_data.surface
        );

        const uint32_t image_count = choose_image_count(surface_capabilities);
        const vk::SurfaceFormatKHR format = choose_format
        (
            device_data.physical_device_data.physical_device,
            *surface_data.surface
        );
        const vk::Extent2D extent = choose_extent(surface_capabilities, surface_data.extent);
        surface_data.extent = extent;

        constexpr size_t max_queue_indices = 2;
        std::array<uint32_t, max_queue_indices> queue_indices_storage;
        const auto [sharing_mode, queue_indices] = choose_sharing_mode_and_indices
        (
            device_data,
            queue_indices_storage
        );

        const vk::PresentModeKHR present_mode = choose_present_mode
        (
            device_data.physical_device_data.physical_device,
            *surface_data.surface
        );

        constexpr uint32_t array_layers_count = 1;
        constexpr auto is_clipped = VK_TRUE;

        const vk::SwapchainCreateInfoKHR create_info
        {
            vk::SwapchainCreateFlagsKHR{},
            *surface_data.surface,
            image_count,
            format.format,
            format.colorSpace,
            extent,
            array_layers_count,
            vk::ImageUsageFlagBits::eColorAttachment,
            sharing_mode,
            static_cast<uint32_t>(queue_indices.size()),
            queue_indices.data(),
            surface_capabilities.currentTransform,
            vk::CompositeAlphaFlagBitsKHR::eOpaque,
            present_mode,
            is_clipped
        };

        vk::UniqueSwapchainKHR swapchain = create_swapchain(create_info, *device_data.logical_device);

        std::vector<vk::Image> images = get_swapchain_images(*device_data.logical_device, *swapchain);
        std::vector<vk::UniqueImageView> image_views = create_image_views
        (
            images,
            format.format,
            *device_data.logical_device
        );

        return SwapchainData
        {
            .swapchain = std::move(swapchain),
            .format = format.format,
            .extent = extent,
            .images = std::move(images),
            .image_views = std::move(image_views)
        };
    }
}
