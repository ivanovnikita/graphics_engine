#include "logical_device_factory.hpp"

namespace ge::impl::factory::device::logical::impl
{

    std::vector<std::string> get_available_device_layers(const vk::PhysicalDevice& device)
    {
        return ge::impl::factory::impl::layers_names(device.enumerateDeviceLayerProperties());
    }

} // namespace ge::impl::factory::device::logical::impl
