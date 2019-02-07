#include "ge/render/render_impl.h"
#include "ge/render/factory/instance.h"
#include "ge/render/factory/device/physical.h"
#include "ge/render/factory/device/logical.h"
#include "ge/render/factory/swapchain.h"
#include "ge/render/factory/image_view.h"
#include "ge/render/factory/pipeline/graphics/pipeline.h"
#include "ge/render/factory/framebuffer.h"
#include "ge/render/factory/command_pool.h"
#include "ge/render/factory/command_buffer.h"
#include "ge/render/factory/semaphore.h"
#include "ge/render/factory/fence.h"
#include "ge/render/storage/shaders.h"
#include "ge/render/debug_callback.h"
#include "ge/render/exception.h"

namespace ge
{
    Render::RenderImpl::RenderImpl
    (
        const std::function<SurfaceCreator>& create_surface
      , vk::Extent2D surface_extent
    )
        : surface_extent_(std::move(surface_extent))
    {
        using namespace factory;
        using namespace factory::options;

        constexpr auto options_instance = Instance
        {
            Debug{DebugCallback{ENABLED}, ValidationLayers{ENABLED}}
          , options::Window{ENABLED, WindowType::XCB}
        };

        instance_ = factory::create_instance(options_instance);

        debug_callback_ = create_debug_callback();
        surface_ = vk::UniqueSurfaceKHR(create_surface(*instance_));

        constexpr factory::options::Graphics option_graphics{ENABLED};

        {
            constexpr options::Device options_device
            {
                options_instance.debug.validation_layers
              , option_graphics
              , options::Compute{DISABLED}
              , options::Transfer{DISABLED}
            };

            {
                const auto[physical_device, queue_family_indices] = factory::create_physical_device
                (
                    options_device
                  , *instance_
                  , *surface_
                );
                physical_device_ = physical_device;
                queue_family_indices_ = queue_family_indices;
            }

            logical_device_ = factory::create_logical_device
            (
                options_instance.debug.validation_layers
              , physical_device_
              , queue_family_indices_
            );

            if constexpr (options_device.graphics.enabled)
            {
                queues_.graphics = logical_device_->getQueue(queue_family_indices_.graphics.value(), 0);
                queues_.present = logical_device_->getQueue(queue_family_indices_.present.value(), 0);
            }

            if constexpr (options_device.compute.enabled)
            {
                queues_.compute = logical_device_->getQueue(queue_family_indices_.compute.value(), 0);
            }

            if constexpr (options_device.transfer.enabled)
            {
                queues_.transfer = logical_device_->getQueue(queue_family_indices_.transfer.value(), 0);
            }
        }

        if constexpr (option_graphics.enabled)
        {
            auto[swapchain, format] = factory::create_swapchain
            (
                physical_device_
              , *logical_device_
              , surface_extent_
              , *surface_
              , queue_family_indices_
            );
            swapchain_ = std::move(swapchain);

            images_ = logical_device_->getSwapchainImagesKHR(*swapchain_);
            image_views_ = factory::create_image_view(images_, format, *logical_device_);
            storage::Shaders shaders(*logical_device_, "shaders");
            auto[pipeline, layout, render_pass] = factory::create_graphics_pipeline
            (
                *logical_device_
              , format
              , shaders
              , surface_extent_
            );
            pipeline_ = std::move(pipeline);
            pipeline_layout_ = std::move(layout);
            render_pass_ = std::move(render_pass);

            framebuffers_.reserve(image_views_.size());
            for (const auto& image_view : image_views_)
            {
                framebuffers_.emplace_back(factory::create_framebuffer
                (
                    *logical_device_
                  , *render_pass_
                  , *image_view
                  , surface_extent_
                ));
            }

            command_pool_ = factory::create_command_pool(*logical_device_, queue_family_indices_);
            command_buffers_ = factory::create_command_buffer
            (
                *logical_device_
                , *command_pool_
                , framebuffers_
                , *render_pass_
                , surface_extent_
                , *pipeline_
            );

            image_available_semaphore_ = factory::create_semaphore(*logical_device_);
            render_finished_semaphore_ = factory::create_semaphore(*logical_device_);

            render_finished_fence_ = factory::create_fence(*logical_device_);
        }
    }

    Render::RenderImpl::~RenderImpl()
    {
        logical_device_->waitIdle();
    }

    vk::UniqueDebugReportCallbackEXT Render::RenderImpl::create_debug_callback() const
    {
        const vk::DebugReportCallbackCreateInfoEXT create_info
        (
            vk::DebugReportFlagBitsEXT::eError
          | vk::DebugReportFlagBitsEXT::ePerformanceWarning
          | vk::DebugReportFlagBitsEXT::eWarning
          , debug_callback
        );

        return instance_->createDebugReportCallbackEXTUnique(create_info);
    }

    void Render::RenderImpl::draw_frame()
    {
        constexpr uint64_t timeout = std::numeric_limits<uint64_t>::max();
        const uint32_t image_index = logical_device_->acquireNextImageKHR
        (
            *swapchain_
            , timeout
            , *image_available_semaphore_
            , vk::Fence{nullptr}
        ).value;

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
            .setPCommandBuffers(&command_buffers_[image_index])
            .setSignalSemaphoreCount(signal_semaphores.size())
            .setPSignalSemaphores(signal_semaphores.data());

        constexpr uint32_t submit_info_count = 1;
        queues_.graphics.submit(submit_info_count, &submit_info, *render_finished_fence_);

        const std::array<vk::SwapchainKHR, 1> swapchains{*swapchain_};

        const auto present_info = vk::PresentInfoKHR{}
            .setWaitSemaphoreCount(signal_semaphores.size())
            .setPWaitSemaphores(signal_semaphores.data())
            .setSwapchainCount(swapchains.size())
            .setPSwapchains(swapchains.data())
            .setPImageIndices(&image_index);

        queues_.present.presentKHR(present_info);

        logical_device_->waitForFences(1, &*render_finished_fence_, VK_TRUE, timeout);
        logical_device_->resetFences(1, &*render_finished_fence_);
    }
}