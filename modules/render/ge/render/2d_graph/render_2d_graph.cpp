#include "render_2d_graph.h"
#include "uniform_buffers.h"
#include "descriptor_set_layout.h"
#include "pipeline_layout.h"
#include "descriptor_set.h"
#include "render_pass.h"
#include "pipelines.h"

#include "ge/render/descriptor_pool.h"
#include "ge/render/shader_module.h"
#include "ge/render/framebuffer.h"
#include "ge/render/fence.h"
#include "ge/render/semaphore.h"
#include "ge/render/command_pool.h"

#include "generated_shaders.h"

namespace ge::graph
{
    Render2dGraph::Render2dGraph(const SurfaceParams& surface_params, const Logger& logger)
        : logger_{logger}
        , instance_data_
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
                logger_
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
                logger_
            )
        }
        , descriptor_set_layout_{create_descriptor_set_layout(*device_data_.logical_device)}
        , pipeline_layout_{create_pipeline_layout(*device_data_.logical_device, *descriptor_set_layout_)}
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
        , swapchain_data_{SwapchainData::create_default(device_data_, surface_data_)}
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
                logger_
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
                logger_
            )
        }
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

    Render2dGraph::~Render2dGraph() = default;

    const Camera2d& Render2dGraph::get_camera() const
    {
        return camera_;
    }

    void Render2dGraph::set_camera(Camera2d camera)
    {
        camera_ = std::move(camera);
    }

    void Render2dGraph::set_object_to_draw(const Graph& graph)
    {
        graph_in_device_memory_.emplace
        (
            load_graph_to_device
            (
                device_data_,
                *command_pool_,
                *transfer_finished_fence_,
                graph
            )
        );

        // TODO: create command buffers
    }

    void Render2dGraph::resize(const uint16_t new_surface_width, const uint16_t new_surface_height)
    {
        wait_idle(*device_data_.logical_device);

        device_data_.logical_device->freeCommandBuffers
        (
            *command_pool_
            , static_cast<uint32_t>(command_buffers_.size())
            , command_buffers_.data()
        );
        command_buffers_.clear();

        framebuffers_.clear();
        vertices_pipeline_.reset();
        arcs_pipeline_.reset();
        swapchain_data_.reset();

        surface_data_.extent = vk::Extent2D{}.setWidth(new_surface_width).setHeight(new_surface_height);

        swapchain_data_ = SwapchainData::create_default(device_data_, surface_data_);
        arcs_pipeline_ = create_arcs_pipeline
        (
            device_data_,
            *render_pass_,
            *shaders_.lines_vertex,
            *shaders_.fragment,
            swapchain_data_.extent,
            *pipeline_layout_,
            logger_
        );
        vertices_pipeline_ = create_vertices_pipeline
        (
            device_data_,
            *render_pass_,
            *shaders_.points_vertex,
            *shaders_.fragment,
            swapchain_data_.extent,
            *pipeline_layout_,
            logger_
        );
        framebuffers_ = create_framebuffers
        (
            *device_data_.logical_device,
            *render_pass_,
            swapchain_data_
        );

        camera_.set_surface_sizes(new_surface_width, new_surface_height);
    }

    void Render2dGraph::draw_frame()
    {
    }
}
