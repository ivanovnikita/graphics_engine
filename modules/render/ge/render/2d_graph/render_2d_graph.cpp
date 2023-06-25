#include "render_2d_graph.h"
#include "uniform_buffers.h"
#include "descriptor_set_layout.h"
#include "pipeline_layout.h"
#include "descriptor_set.h"
#include "render_pass.h"
#include "pipelines.h"

#include "ge/render/descriptor_pool.h"
#include "ge/render/shader_module.h"

#include "generated_shaders.h"

namespace ge::graph
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
        , device_data_
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
                DeviceFeaturesFlags
                {
                    DeviceFeatures::SamplerAnisotropy, // ?
                    DeviceFeatures::FillModeNonSolid,
                    DeviceFeatures::WideLines
                },
                instance_data_,
                *surface_data_.surface,
                logger
            )
        }
        , swapchain_data_{SwapchainData::create_default(device_data_, surface_data_)}
        , descriptor_set_layout_{create_descriptor_set_layout(*device_data_.logical_device)}
        , pipeline_layout_{create_pipeline_layout(*device_data_.logical_device, *descriptor_set_layout_)}
        , uniform_buffers_{create_uniform_buffers(device_data_, swapchain_data_.images.size())}
        , descriptor_pool_{create_descriptor_pool(*device_data_.logical_device, swapchain_data_.images.size())}
        , descriptor_sets_
        {
            create_descriptor_sets
            (
                *device_data_.logical_device,
                *descriptor_pool_,
                *descriptor_set_layout_,
                swapchain_data_.images.size(),
                uniform_buffers_
            )
        }
        , render_pass_{create_render_pass(*device_data_.logical_device, swapchain_data_.format)}
        , shaders_
        {
            Shaders
            {
                .lines_vertex = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::line_2d_camera_Vertex)
                ),
                .points_vertex = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::point_2d_camera_Vertex)
                ),
                .fragment = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::simple_color_Fragment)
                )
            }
        }
        , arcs_pipeline_
        {
            create_arcs_pipeline
            (
                device_data_,
                *render_pass_,
                *shaders_.lines_vertex,
                *shaders_.fragment,
                swapchain_data_.extent,
                *pipeline_layout_,
                logger
            )
        }
        , vertices_pipeline_
        {
            create_vertices_pipeline
            (
                device_data_,
                *render_pass_,
                *shaders_.points_vertex,
                *shaders_.fragment,
                swapchain_data_.extent,
                *pipeline_layout_,
                logger
            )
        }
    {
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
