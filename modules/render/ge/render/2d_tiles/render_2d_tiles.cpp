#include "render_2d_tiles.h"
#include "uniform_buffers.h"
#include "descriptor_set_layout.h"
#include "pipeline_layout.h"
#include "descriptor_set.h"
#include "render_pass.h"
#include "pipelines.h"
#include "draw_tiles_commands.h"

#include "ge/render/vulkan_common/descriptor_pool.h"
#include "ge/render/vulkan_common/shader_module.h"
#include "ge/render/vulkan_common/framebuffer.h"
#include "ge/render/vulkan_common/fence.h"
#include "ge/render/vulkan_common/semaphore.h"
#include "ge/render/vulkan_common/command_pool.h"
#include "ge/render/vulkan_common/exception.h"
#include "ge/render/vulkan_common/queue.h"

#include "generated_shaders.h"

namespace ge::tiles
{
    Render2dTiles::Render2dTiles(const SurfaceParams& surface_params, const Logger& logger)
        : logger_{logger}
        , instance_data_
        {
            InstanceData::create_default
            (
                InstanceLayerFlags
                {
#ifdef GE_DEBUG_LAYERS_ENABLED
                    InstanceLayer::VkLayerKhronosValidation
#endif
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
#ifdef GE_DEBUG_LAYERS_ENABLED
                    DeviceLayer::VkLayerKhronosValidation
#endif
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
                .triangles_vertex = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::polygon_2d_camera_Vertex)
                ),
                .lines_vertex = create_shader_module
                (
                    *device_data_.logical_device,
                    get_shader(ShaderName::line_2d_camera_Vertex)
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
        , triangles_pipeline_
        {
            create_triangles_pipeline
            (
                device_data_,
                *render_pass_,
                *shaders_.triangles_vertex,
                *shaders_.fragment,
                swapchain_data_.extent,
                *pipeline_layout_,
                logger_
            )
        }
        , lines_pipeline_
        {
            create_lines_pipeline
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

    Render2dTiles::~Render2dTiles() = default;

    const Camera2d& Render2dTiles::get_camera() const
    {
        return camera_;
    }

    void Render2dTiles::set_camera(Camera2d camera)
    {
        camera_ = std::move(camera);
    }

    void Render2dTiles::set_object_to_draw(const std::span<const Polygons>& polygons)
    {
        polygons_in_device_memory_.emplace
        (
            load_polygons_to_device
            (
                device_data_,
                *command_pool_,
                *transfer_finished_fence_,
                polygons
            )
        );

        create_command_buffers();
    }

    void Render2dTiles::resize(const uint16_t new_surface_width, const uint16_t new_surface_height)
    {
        wait_idle(*device_data_.logical_device);

        if (not command_buffers_.empty())
        {
            command_buffers_.clear();
        }

        framebuffers_.clear();
        triangles_pipeline_.reset();
        lines_pipeline_.reset();
        swapchain_data_.reset();

        surface_data_.extent = vk::Extent2D{}.setWidth(new_surface_width).setHeight(new_surface_height);

        swapchain_data_ = SwapchainData::create_default(device_data_, surface_data_);
        triangles_pipeline_ = create_triangles_pipeline
        (
            device_data_,
            *render_pass_,
            *shaders_.triangles_vertex,
            *shaders_.fragment,
            swapchain_data_.extent,
            *pipeline_layout_,
            logger_
        );
        lines_pipeline_ = create_lines_pipeline
        (
            device_data_,
            *render_pass_,
            *shaders_.lines_vertex,
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

    void Render2dTiles::create_command_buffers()
    {
        assert(polygons_in_device_memory_.has_value());

        if (not command_buffers_.empty())
        {
            wait_idle(*device_data_.logical_device);
            command_buffers_.clear();
        }

        command_buffers_ = draw_tiles_commands
        (
            *device_data_.logical_device,
            *command_pool_,
            framebuffers_,
            *render_pass_,
            swapchain_data_.extent,
            surface_data_.background_color,
            *triangles_pipeline_,
            *lines_pipeline_,
            *pipeline_layout_,
            descriptor_sets_,
           *polygons_in_device_memory_
        );
    }

    void Render2dTiles::update_uniform_buffer(const size_t current_image)
    {
        assert(current_image < swapchain_data_.images.size());

        constexpr uint32_t offset = 0;
        void* const data = map_memory
        (
            *device_data_.logical_device,
            *uniform_buffers_[current_image].device_memory,
            offset,
            sizeof(ViewProj2d),
            vk::MemoryMapFlags{}
        );
        std::memcpy(data, &camera_.get_view_proj_2d(), sizeof(ViewProj2d));
        device_data_.logical_device->unmapMemory(*uniform_buffers_[current_image].device_memory);
    }

    void Render2dTiles::draw_frame()
    {
        assert(not command_buffers_.empty());

        constexpr uint64_t timeout = std::numeric_limits<uint64_t>::max();

        uint32_t image_index = 0;
        const vk::Result next_image_result = static_cast<vk::Result>
        (
            vkAcquireNextImageKHR
            (
                *device_data_.logical_device,
                *swapchain_data_.swapchain,
                timeout,
                *image_available_semaphore_,
                vk::Fence{nullptr},
                &image_index
            )
        );

        switch (next_image_result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eTimeout:
        case vk::Result::eNotReady:
        case vk::Result::eSuboptimalKHR:
        case vk::Result::eErrorOutOfDateKHR: // TODO: recreate swapchain now?
        {
            return;
        }
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorDeviceLost:
        case vk::Result::eErrorSurfaceLostKHR:
            GE_THROW_EXPECTED_RESULT(next_image_result, "Next image acquiring failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(next_image_result, "Next image acquiring failed");
        }
        }

        update_uniform_buffer(image_index);

        reset_fences(*device_data_.logical_device, {&*render_finished_fence_, 1});

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
            .setPCommandBuffers(&*command_buffers_[image_index])
            .setSignalSemaphoreCount(signal_semaphores.size())
            .setPSignalSemaphores(signal_semaphores.data());

        submit(device_data_.graphics_queue, {&submit_info, 1}, *render_finished_fence_);

        const std::array<vk::SwapchainKHR, 1> swapchains{*swapchain_data_.swapchain};

        const auto present_info = vk::PresentInfoKHR{}
            .setWaitSemaphoreCount(signal_semaphores.size())
            .setPWaitSemaphores(signal_semaphores.data())
            .setSwapchainCount(swapchains.size())
            .setPSwapchains(swapchains.data())
            .setPImageIndices(&image_index);

        const vk::Result present_result = static_cast<vk::Result>
        (
            vkQueuePresentKHR
            (
                device_data_.present_queue,
                reinterpret_cast<const VkPresentInfoKHR*>(&present_info)
            )
        );
        switch (present_result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eSuboptimalKHR:
        case vk::Result::eErrorOutOfDateKHR: // TODO: recreate swapchain now?
        {
            return;
        }
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorDeviceLost:
        case vk::Result::eErrorSurfaceLostKHR:
            GE_THROW_EXPECTED_RESULT(next_image_result, "Queue present failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(next_image_result, "Queue present failed");
        }
        }

        wait_for_fences
        (
            *device_data_.logical_device,
            {&*render_finished_fence_, 1},
            true,
            std::chrono::nanoseconds::max()
        );
    }
}
