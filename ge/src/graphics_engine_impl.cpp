#include "graphics_engine_impl.h"
#include "factory/instance.h"
#include "factory/device/physical.h"
#include "factory/device/logical.h"
#include "factory/swapchain.h"
#include "factory/image_view.h"
#include "factory/pipeline/graphics/pipeline.h"
#include "factory/framebuffer.h"
#include "factory/command_pool.h"
#include "factory/command_buffer.h"
#include "factory/semaphore.h"
#include "storage/shaders.h"
#include "debug_callback.h"
#include "exception.h"

namespace ge::impl
{

    GraphicsEngineImpl::GraphicsEngineImpl()
    {
        using namespace factory;
        using namespace factory::options;

        constexpr auto options_instance = Instance
        {
            Debug{DebugCallback{ENABLED}, ValidationLayers{ENABLED}}
          , options::Window{ENABLED, WindowType::XCB}
        };

        instance_ = factory::instance::create(options_instance);

        debug_callback_ = create_debug_callback();
        window_ = ge::impl::Window::create(500, 500);
        surface_ = window_->create_surface(instance_.get());

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
                using namespace factory::device::physical;
                const auto[physical_device, queue_family_indices] = factory::device::physical::create
                (
                    options_device
                  , *instance_
                  , *surface_
                );
                physical_device_ = physical_device;
                queue_family_indices_ = queue_family_indices;
            }

            logical_device_ = factory::device::logical::create
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
            auto[swapchain, format] = factory::swapchain::create
            (
                physical_device_
              , *logical_device_
              , *window_
              , *surface_
              , queue_family_indices_
            );
            swapchain_ = std::move(swapchain);

            images_ = logical_device_->getSwapchainImagesKHR(*swapchain_);
            image_views_ = factory::image_view::create(images_, format, *logical_device_);
            storage::Shaders shaders(*logical_device_, "shaders");
            auto[pipeline, layout, render_pass] = factory::pipeline::graphics::create
            (
                *logical_device_
              , format
              , shaders
              , *window_
            );
            pipeline_ = std::move(pipeline);
            pipeline_layout_ = std::move(layout);
            render_pass_ = std::move(render_pass);

            framebuffers_.reserve(image_views_.size());
            for (const auto& image_view : image_views_)
            {
                framebuffers_.emplace_back(factory::framebuffer::create
                (
                    *logical_device_
                  , *render_pass_
                  , *image_view
                  , vk::Extent2D(window_->extent())
                ));
            }

            command_pool_ = factory::command_pool::create(*logical_device_, queue_family_indices_);
            command_buffers_ = factory::command_buffer::create
            (
                *logical_device_
                , *command_pool_
                , framebuffers_
                , *render_pass_
                , window_->extent()
                , *pipeline_
            );

            image_available_semaphore_ = factory::semaphore::create(*logical_device_);
            render_finished_semaphore_ = factory::semaphore::create(*logical_device_);
        }
    }

    vk::UniqueDebugReportCallbackEXT GraphicsEngineImpl::create_debug_callback() const
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

    void GraphicsEngineImpl::draw_frame()
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
        queues_.graphics.submit(submit_info_count, &submit_info, vk::Fence{nullptr});

        const std::array<vk::SwapchainKHR, 1> swapchains{*swapchain_};

        const auto present_info = vk::PresentInfoKHR{}
            .setWaitSemaphoreCount(signal_semaphores.size())
            .setPWaitSemaphores(signal_semaphores.data())
            .setSwapchainCount(swapchains.size())
            .setPSwapchains(swapchains.data())
            .setPImageIndices(&image_index);

        queues_.present.presentKHR(present_info);
        queues_.present.waitIdle();
    }

    void GraphicsEngineImpl::main_loop()
    {
        window_->start_display();

        while (not window_->closed())
        {
            draw_frame();
            window_->process_events();
        }

        logical_device_->waitIdle();
    }

}
