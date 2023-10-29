#include "sampler.h"
#include "exception.h"

namespace ge
{
    vk::UniqueSampler create_sampler
    (
        const DeviceData& device_data
    )
    {
        assert(device_data.enabled_features.test(DeviceFeatures::SamplerAnisotropy));

        const vk::SamplerCreateInfo create_info = vk::SamplerCreateInfo{}
            .setMagFilter(vk::Filter::eLinear)
            .setMinFilter(vk::Filter::eLinear)
            .setAddressModeU(vk::SamplerAddressMode::eRepeat)
            .setAddressModeV(vk::SamplerAddressMode::eRepeat)
            .setAddressModeW(vk::SamplerAddressMode::eRepeat)

            .setAnisotropyEnable(VK_TRUE) // or false if DeviceFeatures::SamplerAnisotropy not available
            .setMaxAnisotropy(device_data.physical_device_data.properties.limits.maxSamplerAnisotropy) // or minimal value (1.0f)

            .setBorderColor(vk::BorderColor::eIntOpaqueBlack)
            .setUnnormalizedCoordinates(VK_FALSE)
            .setCompareEnable(VK_FALSE)
            .setCompareOp(vk::CompareOp::eAlways)
            .setMipmapMode(vk::SamplerMipmapMode::eLinear)
            .setMipLodBias(0.0f)
            .setMinLod(0.0f)
            .setMaxLod(0.0f);

        vk::Sampler sampler;
        const vk::Result result = device_data.logical_device->createSampler(&create_info, nullptr, &sampler);
        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorInvalidOpaqueCaptureAddressKHR:
            GE_THROW_EXPECTED_RESULT(result, "Sampler creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Sampler creation failed");
        }
        }

        return vk::UniqueSampler
        {
            std::move(sampler),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{*device_data.logical_device}
        };
    }
}
