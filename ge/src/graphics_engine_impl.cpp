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

}
