#pragma once

#include "generated/vulkan_fwds.h"

namespace vk
{
    using Bool32 = uint32_t;

    class DispatchLoaderStatic;

#define VULKAN_HPP_DEFAULT_DISPATCHER_TYPE DispatchLoaderStatic

    template <typename Type, typename Dispatch>
    class UniqueHandle;

    template <typename BitType>
    class Flags;

    class CommandBuffer;
    using UniqueCommandBuffer = UniqueHandle<CommandBuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class Instance;
    class Device;
    class PhysicalDevice;
    class Image;
    class Queue;

    class CommandPool;
    using UniqueCommandPool = UniqueHandle<CommandPool, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class DescriptorSetLayout;
    using UniqueDescriptorSetLayout = UniqueHandle<DescriptorSetLayout, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class PipelineLayout;
    using UniquePipelineLayout = UniqueHandle<PipelineLayout, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class Pipeline;
    using UniquePipeline = UniqueHandle<Pipeline, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class RenderPass;
    using UniqueRenderPass = UniqueHandle<RenderPass, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class DebugReportCallbackEXT;
    using UniqueDebugReportCallbackEXT = UniqueHandle<DebugReportCallbackEXT, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class DescriptorPool;
    using UniqueDescriptorPool = UniqueHandle<DescriptorPool, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class DescriptorSet;
    using UniqueDescriptorSet = UniqueHandle<DescriptorSet, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class Fence;
    using UniqueFence = UniqueHandle<Fence, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class Framebuffer;
    using UniqueFramebuffer = UniqueHandle<Framebuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class ImageView;
    using UniqueImageView = UniqueHandle<ImageView, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class Sampler;
    using UniqueSampler = UniqueHandle<Sampler, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class Semaphore;
    using UniqueSemaphore = UniqueHandle<Semaphore, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class ShaderModule;
    using UniqueShaderModule = UniqueHandle<ShaderModule, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    using FormatFeatureFlags = Flags<FormatFeatureFlagBits>;
    using ImageAspectFlags = Flags<ImageAspectFlagBits>;

    union ClearColorValue;
}
