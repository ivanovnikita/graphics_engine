#include "graphics_engine_impl.h"
#include "factory/instance.h"
#include "factory/device/physical.h"
#include "factory/device/logical.h"
#include "factory/swapchain.h"
#include "factory/image_view.h"
#include "debug_callback.h"
#include "exception.h"

namespace ge::impl
{

    GraphicsEngineImpl::GraphicsEngineImpl()
    {        
        constexpr bool enabled = true;
        constexpr bool disabled = false;
        {
            using namespace factory;
            using namespace factory::instance;
            instance_ = factory::instance::create
            (
                options::Instance
                {
                    options::Debug
                    {
                        options::DebugCallback{enabled}
                      , options::ValidationLayers{enabled}
                    }
                  , options::Window{enabled, options::WindowType::XCB}
                }
            );
        }

        debug_callback_ = create_debug_callback();
        window_ = Window::create(500, 500);
        surface_ = window_->create_surface(instance_.get());

        constexpr factory::options::Graphics option_graphics{enabled};

        {
            using namespace factory;

            constexpr options::Device options_device
            {
                option_graphics
              , options::Compute{disabled}
              , options::Transfer{disabled}
            };

            {
                using namespace factory::device::physical;
                const auto[physical_device, queue_family_indeces] = factory::device::physical::create
                (
                    options_device
                  , *instance_
                  , *surface_
                );
                physical_device_ = physical_device;
                queue_family_indeces_ = queue_family_indeces;
            }

            logical_device_ = factory::device::logical::create(options_device, physical_device_, queue_family_indeces_);

            if constexpr (options_device.graphics.enabled)
            {
                queues_.graphics = logical_device_->getQueue(queue_family_indeces_.graphics, 0);
                queues_.present = logical_device_->getQueue(queue_family_indeces_.present, 0);
            }

            if constexpr (options_device.compute.enabled)
            {
                queues_.compute = logical_device_->getQueue(queue_family_indeces_.compute, 0);
            }

            if constexpr (options_device.transfer.enabled)
            {
                queues_.transfer = logical_device_->getQueue(queue_family_indeces_.transfer, 0);
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
              , queue_family_indeces_
            );
            swapchain_ = std::move(swapchain);

            images_ = logical_device_->getSwapchainImagesKHR(*swapchain_);
            image_views_ = factory::image_view::create(images_, format, *logical_device_);
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
