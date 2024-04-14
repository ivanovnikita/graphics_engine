module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

#include <span>
#include <vector>

export module vulkan_common.descriptor_set;

import vulkan_common.exception;

namespace ge
{
    export std::vector<vk::UniqueDescriptorSet> allocate_descriptor_sets
    (
        const vk::Device&,
        const vk::DescriptorPool&,
        const vk::DescriptorSetLayout&,
        size_t count
    );
}

module : private;

namespace ge
{
    std::vector<vk::UniqueDescriptorSet> allocate_descriptor_sets
    (
        const vk::Device& device,
        const vk::DescriptorPool& pool,
        const vk::DescriptorSetLayout& layout,
        const size_t count
    )
    {
        std::vector<vk::DescriptorSetLayout> layouts;
        try
        {
            layouts.resize(count, layout);
        }
        catch (const std::bad_alloc&)
        {
            GE_THROW_EXPECTED_ERROR("Allocation for descriptor set layouts failed");
        }

        const vk::DescriptorSetAllocateInfo alloc_info
        {
            pool,
            static_cast<uint32_t>(layouts.size()),
            layouts.data()
        };

        std::vector<vk::DescriptorSet> sets;
        try
        {
            sets.resize(layouts.size());
        }
        catch (const std::bad_alloc&)
        {
            GE_THROW_EXPECTED_ERROR("Allocation for descriptor sets failed");
        }

        const vk::Result allocate_result = device.allocateDescriptorSets(&alloc_info, sets.data());
        switch (allocate_result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorFragmentedPool:
        case vk::Result::eErrorOutOfPoolMemory:
            GE_THROW_EXPECTED_RESULT(allocate_result, "Allocation for descriptor sets failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(allocate_result, "Allocation for descriptor sets failed");
        }
        }

        std::vector<vk::UniqueDescriptorSet> unique_sets;
        try
        {
            unique_sets.reserve(sets.size());
        }
        catch (const std::bad_alloc&)
        {
            GE_THROW_EXPECTED_ERROR("Allocation for unique descriptor sets failed");
        }

        vk::PoolFree
        <
            vk::Device,
            vk::DescriptorPool,
            VULKAN_HPP_DEFAULT_DISPATCHER_TYPE
        > deleter{device, pool};

        for (const vk::DescriptorSet& set : sets)
        {
            unique_sets.emplace_back
            (
                vk::UniqueHandle<vk::DescriptorSet, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{set, deleter}
            );
        }

        return unique_sets;
    }
}
