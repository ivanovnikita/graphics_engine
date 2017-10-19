#include "graphics_engine_impl.h"
#include "instance_factory.hpp"
#include "instance_requirements.h"
#include "physical_device_factory.h"
#include "debug_callback.h"
#include "exception.h"

#include <functional>

namespace ge::impl
{

    GraphicsEngineImpl::GraphicsEngineImpl()
        : instance_         (factory::instance::create
                            (
                                get_required_instance_extensions()
                              , get_required_instance_layers()
                            ))
        , debug_callback_   (create_debug_callback())
        , window_           (Window::create())
        , surface_          (create_surface())
        , device_           (factory::device::physical::create(instance_.get(), surface_.get()))
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
