module;

#include "vulkan_common/exception_macro.h"

#include <vulkan/vulkan.hpp>

module render_base;

import vulkan_common.exception;
import vulkan_common.render_pass;
import vulkan_common.framebuffer;
import vulkan_common.fence;
import vulkan_common.semaphore;
import vulkan_common.command_pool;
import vulkan_common.queue;
import vulkan_common.create_antialiasing;
import overloaded;

namespace ge
{
    RenderBase::RenderBase
    (
        const SurfaceParams& surface_params,
        const Logger& logger,
        DeviceFeaturesFlags device_features,
        const vk::Format desired_surface_format,
        const Antialiasing& antialiasing
    )
        : logger_{logger}
        , instance_data_
        {
            InstanceData::create_default
            (
                InstanceLayerFlags
                {
#ifdef GE_DEBUG_LAYERS_ENABLED
                    InstanceLayer::VkLayerKhronosValidation
#endif
                },
                InstanceExtensionFlags
                {
                    InstanceExtension::VkExtDebugReport,
                    InstanceExtension::VkKhrSurface,
                    InstanceExtension::VkKhrXcbSurface
                },
                logger_
            )
        }
        , surface_data_{SurfaceData::create_default(surface_params, *instance_data_.instance)}
        , device_data_
        {
            DeviceData::create_default
            (
                DeviceLayerFlags
                {
#ifdef GE_DEBUG_LAYERS_ENABLED
                    DeviceLayer::VkLayerKhronosValidation
#endif
                },
                DeviceExtensionFlags
                {
                    DeviceExtension::VkKhrSwapchain
                },
                [&device_features, &antialiasing]
                {
                    return std::visit
                    (
                        overloaded
                        {
                            [&device_features] (NoAntialiasing)
                            {
                                return device_features;
                            },
                            [&device_features] (const Msaa& msaa)
                            {
                                if (msaa.enable_sample_shading)
                                {
                                    device_features.set(DeviceFeatures::SampleShading);
                                }

                                return device_features;
                            }
                        },
                        antialiasing
                    );
                } (),
                instance_data_,
                *surface_data_.surface,
                logger_
            )
        }
        , antialiasing_{choose_max(device_data_.physical_device_data, antialiasing, logger_)}
        , image_available_semaphore_{create_semaphore(*device_data_.logical_device)}
        , render_finished_semaphore_{create_semaphore(*device_data_.logical_device)}
        , render_finished_fence_{create_fence(*device_data_.logical_device)}
        , transfer_finished_fence_{create_fence(*device_data_.logical_device)}
        , command_pool_{create_command_pool(device_data_)}
        , desired_surface_format_{desired_surface_format}
        , swapchain_data_{SwapchainData::create_default(device_data_, surface_data_, desired_surface_format_)}
        , depth_buffer_
        {
            DepthBuffer::create
            (
                device_data_,
                Extent<uint32_t>{.width = swapchain_data_.extent.width, .height = swapchain_data_.extent.height},
                sample_count(antialiasing_),
                *command_pool_,
                *transfer_finished_fence_
            )
        }
        , msaa_data_
        {
            try_create_msaa_data
            (
                antialiasing_,
                device_data_,
                Extent<uint32_t>{.width = swapchain_data_.extent.width, .height = swapchain_data_.extent.height},
                swapchain_data_.format
            )
        }
        , render_pass_
        {
            create_render_pass_default
            (
                *device_data_.logical_device,
                swapchain_data_.format,
                depth_buffer_.format,
                antialiasing_
            )
        }
        , framebuffers_
        {
            create_framebuffers
            (
                *device_data_.logical_device,
                *render_pass_,
                swapchain_data_,
                depth_buffer_,
                msaa_data_.has_value()
                    ? std::optional{*msaa_data_->image_view}
                    : std::nullopt
            )
        }
    {
    }

    RenderBase::~RenderBase()
    {
        wait_idle(*device_data_.logical_device);
    }

    void RenderBase::resize(const Extent<uint32_t> new_surface_extent)
    {
        wait_idle(*device_data_.logical_device);

        if (not command_buffers_.empty())
        {
            command_buffers_.clear();
        }

        framebuffers_.clear();
        swapchain_data_.reset();

        surface_data_.extent = vk::Extent2D{}
            .setWidth(new_surface_extent.width)
            .setHeight(new_surface_extent.height);

        swapchain_data_ = SwapchainData::create_default(device_data_, surface_data_, desired_surface_format_);

        depth_buffer_ = DepthBuffer::create
        (
            device_data_,
            Extent<uint32_t>{.width = swapchain_data_.extent.width, .height = swapchain_data_.extent.height},
            sample_count(antialiasing_),
            *command_pool_,
            *transfer_finished_fence_
        );
        msaa_data_ = try_create_msaa_data
        (
            antialiasing_,
            device_data_,
            Extent<uint32_t>{.width = swapchain_data_.extent.width, .height = swapchain_data_.extent.height},
            swapchain_data_.format
        );

        framebuffers_ = create_framebuffers
        (
            *device_data_.logical_device,
            *render_pass_,
            swapchain_data_,
            depth_buffer_,
            msaa_data_.has_value()
                ? std::optional{*msaa_data_->image_view}
                : std::nullopt
        );

        create_pipelines();

        create_command_buffers();
    }

    void RenderBase::draw_frame()
    {
        assert(not command_buffers_.empty());

        constexpr uint64_t timeout = std::numeric_limits<uint64_t>::max();

        uint32_t image_index = 0;
        const vk::Result next_image_result = static_cast<vk::Result>
        (
            vkAcquireNextImageKHR
            (
                *device_data_.logical_device,
                *swapchain_data_.swapchain,
                timeout,
                *image_available_semaphore_,
                vk::Fence{nullptr},
                &image_index
            )
        );

        switch (next_image_result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eTimeout:
        case vk::Result::eNotReady:
        case vk::Result::eSuboptimalKHR:
        case vk::Result::eErrorOutOfDateKHR: // TODO: recreate swapchain now?
        {
            return;
        }
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorDeviceLost:
        case vk::Result::eErrorSurfaceLostKHR:
            GE_THROW_EXPECTED_RESULT(next_image_result, "Next image acquiring failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(next_image_result, "Next image acquiring failed");
        }
        }

        update_uniform_buffer(image_index);

        reset_fences(*device_data_.logical_device, {&*render_finished_fence_, 1});

        const std::array<vk::Semaphore, 1> wait_semaphores{*image_available_semaphore_};
        const std::array<vk::PipelineStageFlags, 1> wait_stages
        {
            vk::PipelineStageFlagBits::eColorAttachmentOutput
        };
        const std::array<vk::Semaphore, 1> signal_semaphores{*render_finished_semaphore_};
        const auto submit_info = vk::SubmitInfo{}
            .setWaitSemaphoreCount(wait_semaphores.size())
            .setPWaitSemaphores(wait_semaphores.data())
            .setPWaitDstStageMask(wait_stages.data())
            .setCommandBufferCount(1)
            .setPCommandBuffers(&*command_buffers_[image_index])
            .setSignalSemaphoreCount(signal_semaphores.size())
            .setPSignalSemaphores(signal_semaphores.data());

        submit(device_data_.graphics_queue, {&submit_info, 1}, *render_finished_fence_);

        const std::array<vk::SwapchainKHR, 1> swapchains{*swapchain_data_.swapchain};

        const auto present_info = vk::PresentInfoKHR{}
            .setWaitSemaphoreCount(signal_semaphores.size())
            .setPWaitSemaphores(signal_semaphores.data())
            .setSwapchainCount(swapchains.size())
            .setPSwapchains(swapchains.data())
            .setPImageIndices(&image_index);

        const vk::Result present_result = static_cast<vk::Result>
        (
            vkQueuePresentKHR
            (
                device_data_.present_queue,
                reinterpret_cast<const VkPresentInfoKHR*>(&present_info)
            )
        );
        switch (present_result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eSuboptimalKHR:
        case vk::Result::eErrorOutOfDateKHR: // TODO: recreate swapchain now?
        {
            return;
        }
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorDeviceLost:
        case vk::Result::eErrorSurfaceLostKHR:
            GE_THROW_EXPECTED_RESULT(next_image_result, "Queue present failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(next_image_result, "Queue present failed");
        }
        }

        wait_for_fences
        (
            *device_data_.logical_device,
            {&*render_finished_fence_, 1},
            true,
            std::chrono::nanoseconds::max()
        );
    }
}
