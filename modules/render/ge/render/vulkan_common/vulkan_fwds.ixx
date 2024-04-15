export module vulkan_common.vulkan_fwds;

export import vulkan_fwds;

export namespace vk
{
    class DispatchLoaderStatic;

#define VULKAN_HPP_DEFAULT_DISPATCHER_TYPE DispatchLoaderStatic

    template <typename Type, typename Dispatch>
    class UniqueHandle;

    class CommandBuffer;
    using UniqueCommandBuffer = UniqueHandle<CommandBuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;

    class Device;
    class Fence;

    class CommandPool;
    using UniqueCommandPool = UniqueHandle<CommandPool, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>;
}
