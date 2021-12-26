#include "device.h"
#include "ge/render/exception.h"

#include <cinttypes>

namespace ge
{
    namespace
    {
        std::string_view to_string_view(const vk::PhysicalDeviceType& type) noexcept
        {
            switch (type)
            {
            case vk::PhysicalDeviceType::eOther: return "other";
            case vk::PhysicalDeviceType::eIntegratedGpu: return "integrated gpu";
            case vk::PhysicalDeviceType::eDiscreteGpu: return "discrete gpu";
            case vk::PhysicalDeviceType::eVirtualGpu: return "virtual gpu";
            case vk::PhysicalDeviceType::eCpu: return "cpu";
            }
            __builtin_unreachable();
        }

//        std::string_view to_string_view(const vk::SampleCountFlagBits&) noexcept
//        {
//            return "TODO";
//        }

//        void log_sample_count_flags(const vk::SampleCountFlags&, const Logger&) noexcept
//        {
//        }

        void log_device_details(const vk::PhysicalDevice& device, const Logger& logger) noexcept
        {
            const vk::PhysicalDeviceProperties& properties = device.getProperties();
            logger.log
            (
                LogType::SystemInfo,
                "Device properties:\n"
                "- api version: ", properties.apiVersion,
                 " (", VK_API_VERSION_MAJOR(properties.apiVersion),
                 ".", VK_API_VERSION_MINOR(properties.apiVersion),
                 ".", VK_API_VERSION_PATCH(properties.apiVersion),
                 "-", VK_API_VERSION_VARIANT(properties.apiVersion), ")\n"
                "- driver version: ", properties.driverVersion, "\n"
                "- vendor id: ", properties.vendorID, "\n"
                "- device id: ", properties.deviceID, "\n"
                "- device type: ", to_string_view(properties.deviceType), "\n"
                "- device name: ", properties.deviceName.data(), "\n"
                "- limits:\n"
            );

            const vk::PhysicalDeviceLimits& limits = properties.limits;
            logger.log
            (
                LogType::SystemInfo,
                limits.maxImageDimension1D,
                limits.maxImageDimension2D,
                limits.maxImageDimension3D,
                limits.maxImageDimensionCube,
                limits.maxImageArrayLayers,

                limits.maxTexelBufferElements,
                limits.maxUniformBufferRange,
                limits.maxStorageBufferRange,
                limits.maxPushConstantsSize,
                limits.maxMemoryAllocationCount,
                limits.bufferImageGranularity,
                limits.sparseAddressSpaceSize,

                limits.maxBoundDescriptorSets,
                limits.maxPerStageDescriptorSamplers,
                limits.maxPerStageDescriptorUniformBuffers,
                limits.maxPerStageDescriptorSampledImages,
                limits.maxPerStageDescriptorStorageImages,
                limits.maxPerStageDescriptorInputAttachments,
                limits.maxPerStageResources,

                limits.maxDescriptorSetSamplers,
                limits.maxDescriptorSetUniformBuffers,
                limits.maxDescriptorSetUniformBuffersDynamic,
                limits.maxDescriptorSetStorageBuffers,
                limits.maxDescriptorSetStorageBuffersDynamic,
                limits.maxDescriptorSetSampledImages,
                limits.maxDescriptorSetStorageImages,
                limits.maxDescriptorSetInputAttachments,

                limits.maxVertexInputAttributes,
                limits.maxVertexInputBindings,
                limits.maxVertexInputAttributeOffset,
                limits.maxVertexInputBindingStride,
                limits.maxVertexOutputComponents,

                limits.maxTessellationGenerationLevel,
                limits.maxTessellationPatchSize,
                limits.maxTessellationControlPerVertexInputComponents,
                limits.maxTessellationControlPerVertexOutputComponents,
                limits.maxTessellationControlPerPatchOutputComponents,
                limits.maxTessellationControlTotalOutputComponents,
                limits.maxTessellationEvaluationInputComponents,
                limits.maxTessellationEvaluationOutputComponents,

                limits.maxGeometryShaderInvocations,
                limits.maxGeometryInputComponents,
                limits.maxGeometryOutputComponents,
                limits.maxGeometryOutputVertices,
                limits.maxGeometryTotalOutputComponents,

                limits.maxFragmentInputComponents,
                limits.maxFragmentOutputAttachments,
                limits.maxFragmentDualSrcAttachments,
                limits.maxFragmentCombinedOutputResources,

                limits.maxComputeSharedMemorySize,
                limits.maxComputeWorkGroupCount,
                limits.maxComputeWorkGroupInvocations,
                limits.maxComputeWorkGroupSize,

                limits.subPixelPrecisionBits,
                limits.subTexelPrecisionBits,
                limits.mipmapPrecisionBits,

                limits.maxDrawIndexedIndexValue,
                limits.maxDrawIndirectCount,
                limits.maxSamplerLodBias,
                limits.maxSamplerAnisotropy,

                limits.maxViewports,
                limits.maxViewportDimensions,
                limits.viewportBoundsRange,
                limits.viewportSubPixelBits,

                limits.minMemoryMapAlignment,
                limits.minTexelBufferOffsetAlignment,
                limits.minUniformBufferOffsetAlignment,
                limits.minStorageBufferOffsetAlignment,

                limits.minTexelOffset,
                limits.maxTexelOffset,
                limits.minTexelGatherOffset,
                limits.maxTexelGatherOffset,
                limits.minInterpolationOffset,
                limits.maxInterpolationOffset,
                limits.subPixelInterpolationOffsetBits,

                limits.maxFramebufferWidth,
                limits.maxFramebufferHeight,
                limits.maxFramebufferLayers,

//                limits.framebufferColorSampleCounts,
//                limits.framebufferDepthSampleCounts,
//                limits.framebufferStencilSampleCounts,
//                limits.framebufferNoAttachmentsSampleCounts,

                limits.maxColorAttachments,

//                limits.sampledImageColorSampleCounts,
//                limits.sampledImageIntegerSampleCounts,
//                limits.sampledImageDepthSampleCounts,
//                limits.sampledImageStencilSampleCounts,
//                limits.storageImageSampleCounts,

                limits.maxSampleMaskWords,
                limits.timestampComputeAndGraphics,
                limits.timestampPeriod,

                limits.maxClipDistances,
                limits.maxCullDistances,
                limits.maxCombinedClipAndCullDistances,

                limits.discreteQueuePriorities,
                limits.pointSizeRange,
                limits.lineWidthRange,
                limits.pointSizeGranularity,
                limits.lineWidthGranularity,
                limits.strictLines,

                limits.standardSampleLocations,
                limits.optimalBufferCopyOffsetAlignment,
                limits.optimalBufferCopyRowPitchAlignment,
                limits.nonCoherentAtomSize
            );

//            const vk::PhysicalDeviceSparseProperties& sparse_properites = properties.sparseProperties;
        }

        std::vector<vk::PhysicalDevice> get_physical_devices
        (
            const vk::Instance& instance,
            const Logger& logger
        )
        {
            uint32_t devices_count = 0;
            const vk::Result device_count_enumeration_result = instance.enumeratePhysicalDevices
            (
                &devices_count,
                nullptr
            );
            switch (device_count_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorInitializationFailed:
                GE_THROW_EXPECTED_RESULT(device_count_enumeration_result, "Counting physical devices failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(device_count_enumeration_result, "Counting physical devices failed");
            }
            }

            if (devices_count == 0)
            {
                GE_THROW_EXPECTED_ERROR("There is no physical devices at all");
            }

            std::vector<vk::PhysicalDevice>  devices;
            try
            {
                devices.resize(devices_count);
            }
            catch (const std::bad_alloc& e)
            {
                GE_THROW_EXPECTED_ERROR("Allocation for physical devices failed");
            }

            const vk::Result device_enumeration_result = instance.enumeratePhysicalDevices
            (
                &devices_count,
                devices.data()
            );
            switch (device_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorInitializationFailed:
                GE_THROW_EXPECTED_RESULT(device_enumeration_result, "Enumerating physical devices failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(device_enumeration_result, "Enumerating physical devices failed");
            }
            }

            if (logger.enabled(LogType::SystemInfo))
            {
                for (const vk::PhysicalDevice& device : devices)
                {
                    log_device_details(device, logger);
                }
            }

            return devices;
        }
    }

    void DeviceData::create_default
    (
        const factory::options::ValidationLayers&,
        const InstanceData& instance,
        const Logger& logger
    )
    {
        get_physical_devices(*instance.instance, logger);
    }
}
