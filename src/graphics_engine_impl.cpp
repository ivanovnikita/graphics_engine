#include "graphics_engine_impl.h"
#include "instance_factory.h"
#include "physical_device_factory.h"
#include "logical_device_factory.h"
#include "swapchain_factory.h"
#include "image_view_factory.h"
#include "debug_callback.h"
#include "exception.h"

namespace ge::impl
{

    GraphicsEngineImpl::GraphicsEngineImpl()
    {        
        const bool enabled = true;
        const bool disabled = false;
        {
            using namespace factory;
            using namespace factory::instance;
            instance_ = factory::instance::create
            (
                OptionsInstance
                {
                    OptionsDebug
                    {
                        OptionDebugCallback{enabled}
                      , OptionValidationLayers{enabled}
                    }
                  , OptionWindow{enabled, WindowType::XCB}
                }
            );
        }

        debug_callback_ = create_debug_callback();
        window_ = Window::create(500, 500);
        surface_ = create_surface();

        {
            using namespace factory::device::physical;
            const auto[physical_device, queue_family_indeces] = factory::device::physical::create
            (
                OptionsPhysicalDevice
                {
                    OptionGraphics{enabled}
                  , OptionCompute{enabled}
                  , OptionTransfer{enabled}
                }
              , *instance_
              , *surface_
            );
            physical_device_ = physical_device;
            queue_family_indeces_ = queue_family_indeces;
        }

        logical_device_ = factory::device::logical::create(physical_device_, queue_family_indeces_);

        queues_.graphics = logical_device_->getQueue(queue_family_indeces_.graphics, 0);
        queues_.present = logical_device_->getQueue(queue_family_indeces_.present, 0);
        queues_.compute = logical_device_->getQueue(queue_family_indeces_.compute, 0);
        queues_.transfer = logical_device_->getQueue(queue_family_indeces_.transfer, 0);

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

    vk::UniqueSurfaceKHR GraphicsEngineImpl::create_surface() const
    {
        return vk::UniqueSurfaceKHR(std::move(*(window_->create_surface(instance_).release())));
    }

} // namespace ge::impl
