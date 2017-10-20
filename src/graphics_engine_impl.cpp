#include "graphics_engine_impl.h"
#include "instance_factory.hpp"
#include "instance_requirements.h"
#include "physical_device_factory.h"
#include "logical_device_factory.hpp"
#include "device_tools.h"
#include "debug_callback.h"
#include "exception.h"

namespace ge::impl
{

    GraphicsEngineImpl::GraphicsEngineImpl()
        : instance_             (factory::instance::create
                                (
                                    get_required_instance_extensions()
                                  , get_required_instance_layers()
                                ))
        , debug_callback_       (create_debug_callback())
        , window_               (Window::create())
        , surface_              (create_surface())
        , physical_device_      (factory::device::physical::create(*instance_, *surface_))
        , queue_family_index_   (static_cast<uint32_t>(factory::device::get_suitable_queue_family_index
                                (
                                    physical_device_
                                  , *surface_
                                )))
        , logical_device_       (factory::device::logical::create
                                (
                                    physical_device_
                                  , get_required_instance_layers()
                                  , queue_family_index_
                                ))
        , queue_                (logical_device_->getQueue(queue_family_index_, 0))
    {
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
