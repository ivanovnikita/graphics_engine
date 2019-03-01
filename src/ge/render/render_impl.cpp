#include "ge/render/render_impl.h"
#include "ge/render/factory/instance.h"
#include "ge/render/factory/device/physical.h"
#include "ge/render/factory/device/logical.h"
#include "ge/render/factory/swapchain.h"
#include "ge/render/factory/image_view.h"
#include "ge/render/factory/pipeline/graphics/pipeline.h"
#include "ge/render/factory/framebuffer.h"
#include "ge/render/factory/command_pool.h"
#include "ge/render/factory/command_buffer.h"
#include "ge/render/factory/semaphore.h"
#include "ge/render/factory/fence.h"
#include "ge/render/factory/buffer.h"
#include "ge/render/storage/shaders.h"
#include "ge/render/debug_callback.h"
#include "ge/render/exception.h"
#include "ge/render/utils/safe_cast.hpp"
#include "ge/render/vertex.h"

namespace ge
{
    namespace
    {
        const std::array VERTICES
        {
            Vertex{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}
            , Vertex{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}
            , Vertex{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        void copy_buffer
        (
            const vk::Buffer& src
            , const vk::Buffer& dst
            , const size_t size
            , const vk::Device& logical_device
            , const vk::CommandPool& command_pool
            , const vk::Queue& graphics_queue
        )
        {
            const auto alloc_info = vk::CommandBufferAllocateInfo{}
                .setLevel(vk::CommandBufferLevel::ePrimary)
                .setCommandPool(command_pool)
                .setCommandBufferCount(1);

            const std::vector<vk::UniqueCommandBuffer> command_buffers = logical_device.allocateCommandBuffersUnique
            (
                alloc_info
            );
            const vk::CommandBuffer& command_buffer = *command_buffers.front();

            const auto begin_info = vk::CommandBufferBeginInfo{}
                .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

            command_buffer.begin(begin_info);
            const auto copy_region = vk::BufferCopy{}
                .setSrcOffset(0)
                .setDstOffset(0)
                .setSize(size);
            command_buffer.copyBuffer(src, dst, copy_region);
            command_buffer.end();

            const auto submit_info = vk::SubmitInfo{}
                .setCommandBufferCount(1)
                .setPCommandBuffers(&command_buffer);

            graphics_queue.submit(1, &submit_info, nullptr);
            graphics_queue.waitIdle();
        }
    }

    Render::RenderImpl::RenderImpl
    (
        const std::function<SurfaceCreator>& create_surface
      , vk::Extent2D surface_extent
    )
        : surface_extent_(std::move(surface_extent))
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
        surface_ = vk::UniqueSurfaceKHR(create_surface(*instance_));

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

            create_graphics_pipeline();

            image_available_semaphore_ = factory::create_semaphore(*logical_device_);
            render_finished_semaphore_ = factory::create_semaphore(*logical_device_);

            render_finished_fence_ = factory::create_fence(*logical_device_);
        }
    }

    Render::RenderImpl::~RenderImpl()
    {
        logical_device_->waitIdle();
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

        auto[pipeline, layout, render_pass] = factory::create_graphics_pipeline
        (
            *logical_device_
          , format
          , shaders_storage_
          , surface_extent_
        );
        pipeline_ = std::move(pipeline);
        pipeline_layout_ = std::move(layout);
        render_pass_ = std::move(render_pass);

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

        command_pool_ = factory::create_command_pool(*logical_device_, queue_family_indices_);

        {
            const uint32_t buffer_size = sizeof(Vertex) * VERTICES.size();

            auto [staging_buffer, staging_memory] = factory::create_buffer
            (
                physical_device_
                , *logical_device_
                , buffer_size
                , vk::BufferUsageFlagBits::eTransferSrc
                , vk::MemoryPropertyFlagBits::eHostVisible
                | vk::MemoryPropertyFlagBits::eHostCoherent
            );

            constexpr uint32_t offset = 0;
            void* data = logical_device_->mapMemory
            (
                *staging_memory
                , offset
                , buffer_size
                , vk::MemoryMapFlags{}
            );
            std::memcpy(data, VERTICES.data(), static_cast<size_t>(buffer_size));
            logical_device_->unmapMemory(*staging_memory);

            auto [vertex_buffer, vertex_memory] = factory::create_buffer
            (
                physical_device_
                , *logical_device_
                , buffer_size
                , vk::BufferUsageFlagBits::eTransferDst
                | vk::BufferUsageFlagBits::eVertexBuffer
                , vk::MemoryPropertyFlagBits::eDeviceLocal
            );
            vertex_buffer_memory_ = std::move(vertex_memory);
            vertex_buffer_ = std::move(vertex_buffer);

            copy_buffer
            (
                *staging_buffer
                , *vertex_buffer_
                , buffer_size
                , *logical_device_
                , *command_pool_
                , queues_.graphics
            );
        }

        command_buffers_ = factory::create_command_buffer
        (
            *logical_device_
            , *command_pool_
            , framebuffers_
            , *render_pass_
            , surface_extent_
            , *pipeline_
            , *vertex_buffer_
            , VERTICES
        );
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
        framebuffers_.clear();
        pipeline_.reset();
        pipeline_layout_.reset();
        render_pass_.reset();
        image_views_.clear();
        images_.clear();
        swapchain_.reset();

        surface_extent_ = vk::Extent2D{}.setWidth(new_surface_width).setHeight(new_surface_height);

        create_graphics_pipeline();
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

    void Render::RenderImpl::draw_frame()
    {
        constexpr uint64_t timeout = std::numeric_limits<uint64_t>::max();
        const vk::ResultValue<uint32_t> image_index_result = logical_device_->acquireNextImageKHR
        (
            *swapchain_
            , timeout
            , *image_available_semaphore_
            , vk::Fence{nullptr}
        );

        if
        (
            image_index_result.result == vk::Result::eErrorOutOfDateKHR
            or image_index_result.result == vk::Result::eSuboptimalKHR
        )
        {
            return;
        }

        const uint32_t image_index = image_index_result.value;

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
