#include "ge/common/safe_cast.hpp"
#include "ge/render/render_impl.h"
#include "ge/render/factory/instance.h"
#include "ge/render/factory/device/physical.h"
#include "ge/render/factory/device/logical.h"
#include "ge/render/factory/swapchain.h"
#include "ge/render/factory/image_view.h"
#include "ge/render/factory/descriptor_set_layout.h"
#include "ge/render/factory/descriptor_pool.h"
#include "ge/render/factory/pipeline/graphics/render_pass.h"
#include "ge/render/factory/pipeline/graphics/pipeline.h"
#include "ge/render/factory/framebuffer.h"
#include "ge/render/factory/command_pool.h"
#include "ge/render/factory/command_buffer.h"
#include "ge/render/factory/semaphore.h"
#include "ge/render/factory/fence.h"
#include "ge/render/factory/buffer.hpp"
#include "ge/render/storage/shaders.h"
#include "ge/render/debug_callback.h"
#include "ge/render/exception.h"
#include "ge/render/vertex.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

namespace ge
{
    RenderImpl::RenderImpl
    (
        const SurfaceParams& surface_params,
        const DrawMode draw_mode,
        const Logger& logger
    )
        : draw_mode_{draw_mode}
        , instance_data_{InstanceData::create_default(factory::options::Instance::create_default(), logger)}
        , surface_data_{SurfaceData::create_default(surface_params, *instance_data_.instance)}
        , camera_
        {
            {0.f, 0.f},
            1.f,
            surface_params.width,
            surface_params.height
        }
    {
        using namespace factory;
        using namespace factory::options;

        constexpr auto options_instance = Instance
        {
#ifndef NDEBUG
            Debug{DebugCallback{ENABLED}, ValidationLayers{ENABLED}}
#else
            Debug{DebugCallback{DISABLED}, ValidationLayers{DISABLED}}
#endif
          , options::Window{ENABLED, WindowType::XCB}
        };

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
                const auto[physical_device, queue_family_indices] = factory::create_physical_device
                (
                    options_device
                  , *instance_data_.instance
                  , *surface_data_.surface
                );
                physical_device_ = physical_device;
                queue_family_indices_ = queue_family_indices;
            }

            logical_device_ = factory::create_logical_device
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
            shaders_storage_ = storage::Shaders{*logical_device_};

            image_available_semaphore_ = factory::create_semaphore(*logical_device_);
            render_finished_semaphore_ = factory::create_semaphore(*logical_device_);

            render_finished_fence_ = factory::create_fence(*logical_device_);
            transfer_finished_fence_ = factory::create_fence(*logical_device_);

            create_graphics_pipeline();

            command_pool_ = factory::create_command_pool(*logical_device_, queue_family_indices_);
        }
    }

    RenderImpl::~RenderImpl()
    {
        logical_device_->waitIdle();
    }

    // TODO: use one buffer and split it in parts for each image
    void RenderImpl::create_uniform_buffers()
    {
        assert(not images_.empty());

        camera_2d_uniform_buffers_memory_.reserve(images_.size());
        camera_2d_uniform_buffers_.reserve(images_.size());

        constexpr vk::DeviceSize BUFFER_SIZE = sizeof(ViewProj2d);
        for (size_t i = 0; i < images_.size(); ++i)
        {
            auto [buffer, memory] = factory::create_buffer
            (
                physical_device_
                , *logical_device_
                , BUFFER_SIZE
                , vk::BufferUsageFlagBits::eUniformBuffer
                , vk::MemoryPropertyFlagBits::eHostVisible
                | vk::MemoryPropertyFlagBits::eHostCoherent
            );

            camera_2d_uniform_buffers_memory_.emplace_back(std::move(memory));
            camera_2d_uniform_buffers_.emplace_back(std::move(buffer));
        }
    }

    void RenderImpl::create_descriptor_sets()
    {
        std::vector<vk::DescriptorSetLayout> layouts{images_.size(), *camera_2d_descriptor_set_layout_};

        const auto alloc_info = vk::DescriptorSetAllocateInfo{}
            .setDescriptorPool(*descriptor_pool_)
            .setDescriptorSetCount(static_cast<uint32_t>(images_.size()))
            .setPSetLayouts(layouts.data());

        descriptor_sets_ = logical_device_->allocateDescriptorSets(alloc_info);

        assert(descriptor_sets_.size() == camera_2d_uniform_buffers_.size());

        for (size_t i = 0; i < descriptor_sets_.size(); ++i)
        {
            const auto buffer_info = vk::DescriptorBufferInfo{}
                .setBuffer(*camera_2d_uniform_buffers_[i])
                .setOffset(0)
                .setRange(sizeof(ViewProj2d));

            const auto descriptor_write = vk::WriteDescriptorSet{}
                .setDstSet(descriptor_sets_[i])
                .setDstBinding(0)
                .setDstArrayElement(0)
                .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                .setDescriptorCount(1)
                .setPBufferInfo(&buffer_info);

            logical_device_->updateDescriptorSets(1, &descriptor_write, 0, nullptr);
        }
    }

    void RenderImpl::create_graphics_pipeline()
    {
        auto[swapchain, format, extent] = factory::create_swapchain
        (
            physical_device_
          , *logical_device_
          , surface_data_.extent
          , *surface_data_.surface
          , queue_family_indices_
        );
        surface_data_.extent = extent;
        camera_.set_surface_sizes(surface_data_.extent.width, surface_data_.extent.height);

        swapchain_ = std::move(swapchain);
        images_ = logical_device_->getSwapchainImagesKHR(*swapchain_);
        image_views_ = factory::create_image_view(images_, format, *logical_device_);

        camera_2d_descriptor_set_layout_ = factory::camera_2d_descriptor_set_layout(*logical_device_);
        camera_2d_pipeline_layout_ = factory::camera_2d_pipeline_layout
        (
            *logical_device_
            , *camera_2d_descriptor_set_layout_
        );
        create_uniform_buffers();
        descriptor_pool_ = factory::create_descriptor_pool(*logical_device_, images_.size());
        create_descriptor_sets();

        render_pass_ = factory::create_render_pass(*logical_device_, format);
        camera_2d_pipeline_layout_ = factory::camera_2d_pipeline_layout
        (
            *logical_device_
            , *camera_2d_descriptor_set_layout_
        );

        switch (draw_mode_)
        {
        case DrawMode::POLYGONS:
        {
            triangles_pipeline_ = factory::polygon_pipeline
            (
                *logical_device_
              , *render_pass_
              , shaders_storage_
              , surface_data_.extent
              , *camera_2d_pipeline_layout_
            );
            lines_pipeline_ = factory::lines_pipeline
            (
                *logical_device_
              , *render_pass_
              , shaders_storage_
              , surface_data_.extent
              , *camera_2d_pipeline_layout_
            );
            break;
        }
        case DrawMode::GRAPH:
        {
            graph_acrs_pipeline_ = factory::graph_arcs_pipeline
            (
                *logical_device_
              , *render_pass_
              , shaders_storage_
              , surface_data_.extent
              , *camera_2d_pipeline_layout_
            );
            graph_vertices_pipeline_ = factory::graph_vertices_pipeline
            (
                *logical_device_
              , *render_pass_
              , shaders_storage_
              , surface_data_.extent
              , *camera_2d_pipeline_layout_
            );
            break;
        }
        }

        framebuffers_.reserve(image_views_.size());
        for (const auto& image_view : image_views_)
        {
            framebuffers_.emplace_back(factory::create_framebuffer
            (
                *logical_device_
              , *render_pass_
              , *image_view
              , surface_data_.extent
            ));
        }
    }

    void RenderImpl::resize(const uint16_t new_surface_width, const uint16_t new_surface_height)
    {
        logical_device_->waitIdle();

        logical_device_->freeCommandBuffers
        (
            *command_pool_
            , safe_cast<uint32_t>(command_buffers_.size())
            , command_buffers_.data()
        );
        command_buffers_.clear();
        framebuffers_.clear();

        triangles_pipeline_.reset();
        lines_pipeline_.reset();

        graph_acrs_pipeline_.reset();
        graph_vertices_pipeline_.reset();

        camera_2d_pipeline_layout_.reset();
        render_pass_.reset();

        descriptor_pool_.reset();
        camera_2d_uniform_buffers_.clear();
        camera_2d_uniform_buffers_memory_.clear();
        camera_2d_descriptor_set_layout_.reset();

        image_views_.clear();
        images_.clear();
        swapchain_.reset();

        surface_data_.extent = vk::Extent2D{}.setWidth(new_surface_width).setHeight(new_surface_height);
        camera_.set_surface_sizes(surface_data_.extent.width, surface_data_.extent.height);

        create_graphics_pipeline();
        create_command_buffers();
    }

    void RenderImpl::set_object_to_draw(const std::span<const Polygons>& polygons)
    {
        polygon_in_device_mem_ = factory::load_polygons_to_device
        (
            physical_device_
            , *logical_device_
            , *command_pool_
            , queues_.graphics // TODO: transfer queue?
            , *transfer_finished_fence_
            , polygons
        );

        create_command_buffers();
    }

    void RenderImpl::set_object_to_draw(const Graph& graph)
    {
        graph_in_device_mem_ = factory::load_graph_to_device
        (
            physical_device_
            , *logical_device_
            , *command_pool_
            , queues_.graphics // TODO: transfer queue?
            , *transfer_finished_fence_
            , graph
        );

        create_command_buffers();
    }

    void RenderImpl::create_command_buffers()
    {
        if (not command_buffers_.empty())
        {
            logical_device_->waitIdle();
            logical_device_->freeCommandBuffers
            (
                *command_pool_
                , safe_cast<uint32_t>(command_buffers_.size())
                , command_buffers_.data()
            );
            command_buffers_.clear();
        }

        switch (draw_mode_)
        {
        case DrawMode::POLYGONS:
        {
            command_buffers_ = factory::draw_polygons_commands
            (
                *logical_device_
                , *command_pool_
                , framebuffers_
                , *render_pass_
                , surface_data_.extent
                , surface_data_.background_color
                , *triangles_pipeline_
                , *lines_pipeline_
                , *camera_2d_pipeline_layout_
                , descriptor_sets_
                , polygon_in_device_mem_
            );
            break;
        }
        case DrawMode::GRAPH:
        {
            command_buffers_ = factory::draw_graph_commands
            (
                *logical_device_
                , *command_pool_
                , framebuffers_
                , *render_pass_
                , surface_data_.extent
                , surface_data_.background_color
                , *graph_acrs_pipeline_
                , *graph_vertices_pipeline_
                , *camera_2d_pipeline_layout_
                , descriptor_sets_
                , graph_in_device_mem_
            );
            break;
        }
        }
    }

    void RenderImpl::update_uniform_buffer(const size_t current_image_index)
    {
        assert(current_image_index < images_.size());

        constexpr uint32_t offset = 0;
        void* data = logical_device_->mapMemory
        (
            *camera_2d_uniform_buffers_memory_[current_image_index]
            , offset
            , sizeof(ViewProj2d)
            , vk::MemoryMapFlags{}
        );
        std::memcpy(data, &camera_.get_view_proj_2d(), sizeof(ViewProj2d));
        logical_device_->unmapMemory(*camera_2d_uniform_buffers_memory_[current_image_index]);
    }

    const Camera2d& RenderImpl::get_camera() const
    {
        return camera_;
    }

    void RenderImpl::set_camera(Camera2d camera)
    {
        camera_ = std::move(camera);
    }

    void RenderImpl::draw_frame()
    {
        if (command_buffers_.empty())
        {
            throw std::logic_error("there is no object to draw!");
        }

        constexpr uint64_t timeout = std::numeric_limits<uint64_t>::max();

        uint32_t image_index = 0;
        try
        {
            const vk::ResultValue<uint32_t> image_index_result = logical_device_->acquireNextImageKHR
            (
                *swapchain_
                , timeout
                , *image_available_semaphore_
                , vk::Fence{nullptr}
            );

            if (image_index_result.result != vk::Result::eSuccess)
            {
                return;
            }

            image_index = image_index_result.value;
        }
        catch (const vk::OutOfDateKHRError&)
        {
            return;
        }

        update_uniform_buffer(image_index);

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

        logical_device_->resetFences({*render_finished_fence_});
        queues_.graphics.submit({submit_info}, *render_finished_fence_);

        const std::array<vk::SwapchainKHR, 1> swapchains{*swapchain_};

        const auto present_info = vk::PresentInfoKHR{}
            .setWaitSemaphoreCount(signal_semaphores.size())
            .setPWaitSemaphores(signal_semaphores.data())
            .setSwapchainCount(swapchains.size())
            .setPSwapchains(swapchains.data())
            .setPImageIndices(&image_index);

        const vk::Result present_result = queues_.present.presentKHR(&present_info);
        if (present_result == vk::Result::eErrorOutOfDateKHR or present_result == vk::Result::eSuboptimalKHR)
        {
            return;
        }
        else if (present_result != vk::Result::eSuccess)
        {
            vk::throwResultException(present_result, VULKAN_HPP_NAMESPACE_STRING"::Queue::presentKHR");
        }

        if (vk::Result r = logical_device_->waitForFences({*render_finished_fence_}, VK_TRUE, timeout); r != vk::Result::eSuccess)
        {
            vk::throwResultException(r, VULKAN_HPP_NAMESPACE_STRING"::LogicalDevice::wairForFences");
        }
    }
}