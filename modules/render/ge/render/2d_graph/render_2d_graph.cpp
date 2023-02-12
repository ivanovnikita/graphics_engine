#include "render_2d_graph.h"

namespace ge
{
    Render2dGraph::Render2dGraph(const SurfaceParams& surface_params, const Logger& logger)
        : instance_data_
        {
            InstanceData::create_default
            (
                InstanceLayerFlags
                {
                    InstanceLayer::VkLayerKhronosValidation
                },
                InstanceExtensionFlags
                {
                    InstanceExtension::VkExtDebugReport,
                    InstanceExtension::VkKhrSurface,
                    InstanceExtension::VkKhrXcbSurface
                },
                logger
            )
        }
        , surface_data_{SurfaceData::create_default(surface_params, *instance_data_.instance)}
        , camera_
        {
            {0.f, 0.f},
            1.f,
            surface_params.width,
            surface_params.height
        }
    {
        DeviceData::create_default
        (
            DeviceLayerFlags
            {
                DeviceLayer::VkLayerKhronosValidation
            },
            DeviceExtensionFlags
            {
                DeviceExtension::VkKhrSwapchain
            },
            instance_data_,
            *surface_data_.surface,
            logger
        );
    }

    Render2dGraph::~Render2dGraph() = default;

    void Render2dGraph::draw_frame()
    {
    }

    void Render2dGraph::resize(const uint16_t new_surface_width, const uint16_t new_surface_height)
    {
        camera_.set_surface_sizes(new_surface_width, new_surface_height);
    }

    const Camera2d& Render2dGraph::get_camera() const
    {
        return camera_;
    }

    void Render2dGraph::set_camera(Camera2d camera)
    {
        camera_ = std::move(camera);
    }

    void Render2dGraph::set_object_to_draw(const Graph&)
    {
    }
}
