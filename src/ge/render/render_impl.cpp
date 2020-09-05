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
#include "ge/render/utils/safe_cast.hpp"
#include "ge/render/vertex.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

namespace ge
{
    Render::RenderImpl::RenderImpl
    (
        const SurfaceParams& surface_params
    )
        : surface_extent_{vk::Extent2D{}.setWidth(surface_params.width).setHeight(surface_params.height)}
        , surface_background_color_
        {
            // TODO: this conversion may be incorrect, use linear to sRGB colorspace conversion
            std::array<float, 4>
            {
                surface_params.background_color[0] / 255.f
              , surface_params.background_color[1] / 255.f
              , surface_params.background_color[2] / 255.f
              , surface_params.background_color[3] / 255.f
            }
        }
        , camera_{.transform = ViewProj2d{.pos = {0.f, 0.f}, .ortho_proj = {1.f, 1.f}}, .scale = 1.f}
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

        instance_ = factory::create_instance(options_instance);

#ifndef NDEBUG
        debug_callback_ = create_debug_callback();
#endif
        surface_ = vk::UniqueSurfaceKHR(surface_params.surface_creator(*instance_));

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
                  , *instance_
                  , *surface_
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

    Render::RenderImpl::~RenderImpl()
    {
        logical_device_->waitIdle();
    }

    // TODO: use one buffer and split it in parts for each image
    void Render::RenderImpl::create_uniform_buffers()
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

    void Render::RenderImpl::create_descriptor_sets()
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

    void Render::RenderImpl::create_graphics_pipeline()
    {
        auto[swapchain, format, extent] = factory::create_swapchain
        (
            physical_device_
          , *logical_device_
          , surface_extent_
          , *surface_
          , queue_family_indices_
        );
        surface_extent_ = extent;
        swapchain_ = std::move(swapchain);
        images_ = logical_device_->getSwapchainImagesKHR(*swapchain_);
        image_views_ = factory::create_image_view(images_, format, *logical_device_);

        update_camera_transform();

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
        triangles_pipeline_ = factory::polygon_pipeline
        (
            *logical_device_
          , *render_pass_
          , shaders_storage_
          , surface_extent_
          , *camera_2d_pipeline_layout_
        );
        lines_pipeline_ = factory::lines_pipeline
        (
            *logical_device_
          , *render_pass_
          , shaders_storage_
          , surface_extent_
          , *camera_2d_pipeline_layout_
        );

        framebuffers_.reserve(image_views_.size());
        for (const auto& image_view : image_views_)
        {
            framebuffers_.emplace_back(factory::create_framebuffer
            (
                *logical_device_
              , *render_pass_
              , *image_view
              , surface_extent_
            ));
        }
    }

    void Render::RenderImpl::resize(const uint16_t new_surface_width, const uint16_t new_surface_height)
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
        camera_2d_pipeline_layout_.reset();
        render_pass_.reset();

        descriptor_pool_.reset();
        camera_2d_uniform_buffers_.clear();
        camera_2d_uniform_buffers_memory_.clear();
        camera_2d_descriptor_set_layout_.reset();

        image_views_.clear();
        images_.clear();
        swapchain_.reset();

        surface_extent_ = vk::Extent2D{}.setWidth(new_surface_width).setHeight(new_surface_height);

        create_graphics_pipeline();
        create_command_buffers();
    }

    vk::UniqueDebugReportCallbackEXT Render::RenderImpl::create_debug_callback() const
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

    void Render::RenderImpl::set_object_to_draw(const Polygons& polygons)
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

    void Render::RenderImpl::create_command_buffers()
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

        command_buffers_ = factory::draw_polygons_commands
        (
            *logical_device_
            , *command_pool_
            , framebuffers_
            , *render_pass_
            , surface_extent_
            , surface_background_color_
            , *triangles_pipeline_
            , *lines_pipeline_
            , *camera_2d_pipeline_layout_
            , descriptor_sets_
            , polygon_in_device_mem_
        );
    }

    void Render::RenderImpl::update_uniform_buffer(const size_t current_image_index)
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
        std::memcpy(data, &camera_.transform, sizeof(ViewProj2d));
        logical_device_->unmapMemory(*camera_2d_uniform_buffers_memory_[current_image_index]);
    }

    glm::vec2 Render::RenderImpl::camera_pos() const
    {
        return camera_.transform.pos;
    }

    void Render::RenderImpl::set_camera_pos(const glm::vec2& pos)
    {
        camera_.transform.pos = pos;
    }

    float Render::RenderImpl::camera_scale() const
    {
        return camera_.scale;
    }

    void Render::RenderImpl::set_camera_scale(const float scale)
    {
        camera_.scale = scale;
        update_camera_transform();
    }

    void Render::RenderImpl::update_camera_transform()
    {
        camera_.transform.ortho_proj.x = (1.f / static_cast<float>(surface_extent_.width)) / camera_.scale;
        camera_.transform.ortho_proj.y = (1.f / static_cast<float>(surface_extent_.height)) / camera_.scale;
    }

    glm::vec2 Render::RenderImpl::normalize_in_proj_space(const glm::vec2& coord) const
    {
        glm::vec2 result{coord};

        glm::vec2 surface_center{surface_extent_.width / 2, surface_extent_.height / 2};

        result.x -= surface_center.x;
        result.x /= surface_center.x;

        result.y -= surface_center.y;
        result.y /= surface_center.y;

        return result;
    }

    // TODO: write tests
    glm::vec2 Render::RenderImpl::proj_to_model_space(const glm::vec2& coord) const
    {
        glm::vec2 result{coord.x, -coord.y};

        result.x /= camera_.transform.ortho_proj.x;
        result.y /= camera_.transform.ortho_proj.y;

        result += camera_.transform.pos;

        return result;
    }

    void Render::RenderImpl::draw_frame()
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

        constexpr uint32_t submit_info_count = 1;
        logical_device_->resetFences(1, &*render_finished_fence_);
        queues_.graphics.submit(submit_info_count, &submit_info, *render_finished_fence_);

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

        logical_device_->waitForFences(1, &*render_finished_fence_, VK_TRUE, timeout);
    }
}
