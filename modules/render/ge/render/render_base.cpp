#include "render_base.h"

#include "ge/render/vulkan_common/exception.h"
#include "ge/render/vulkan_common/render_pass.h"
#include "ge/render/vulkan_common/framebuffer.h"
#include "ge/render/vulkan_common/fence.h"
#include "ge/render/vulkan_common/semaphore.h"
#include "ge/render/vulkan_common/command_pool.h"
#include "ge/render/vulkan_common/queue.h"

namespace ge
{
    RenderBase::RenderBase
    (
        const SurfaceParams& surface_params,
        const Logger& logger,
        DeviceFeaturesFlags device_feartures,
        const vk::Format desired_surface_format
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
                std::move(device_feartures),
                instance_data_,
                *surface_data_.surface,
                logger_
            )
        }
        , desired_surface_format_{desired_surface_format}
        , swapchain_data_{SwapchainData::create_default(device_data_, surface_data_, desired_surface_format_)}
        , render_pass_{create_render_pass_default(*device_data_.logical_device, swapchain_data_.format)}
        , framebuffers_
        {
            create_framebuffers
            (
                *device_data_.logical_device,
                *render_pass_,
                swapchain_data_
            )
        }
        , image_available_semaphore_{create_semaphore(*device_data_.logical_device)}
        , render_finished_semaphore_{create_semaphore(*device_data_.logical_device)}
        , render_finished_fence_{create_fence(*device_data_.logical_device)}
        , transfer_finished_fence_{create_fence(*device_data_.logical_device)}
        , command_pool_{create_command_pool(device_data_)}
    {
    }

    RenderBase::~RenderBase()
    {
        wait_idle(*device_data_.logical_device);
    }

    void RenderBase::resize(const uint16_t new_surface_width, const uint16_t new_surface_height)
    {
        wait_idle(*device_data_.logical_device);

        if (not command_buffers_.empty())
        {
            command_buffers_.clear();
        }

        framebuffers_.clear();
        swapchain_data_.reset();

        surface_data_.extent = vk::Extent2D{}.setWidth(new_surface_width).setHeight(new_surface_height);

        swapchain_data_ = SwapchainData::create_default(device_data_, surface_data_, desired_surface_format_);
        framebuffers_ = create_framebuffers
        (
            *device_data_.logical_device,
            *render_pass_,
            swapchain_data_
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
