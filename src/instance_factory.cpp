#include "instance_factory.hpp"

namespace ge::impl::factory::instance::impl
{

    std::vector<std::string> get_available_instance_extensions()
    {
        return ge::impl::factory::impl::extensions_names(vk::enumerateInstanceExtensionProperties());
    }

    std::vector<std::string> get_available_instance_layers()
    {
        return ge::impl::factory::impl::layers_names(vk::enumerateInstanceLayerProperties());
    }

} // namespace ge::impl::factory::instance::impl
