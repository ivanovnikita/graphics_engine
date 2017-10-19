#include "instance_factory.hpp"

namespace ge::impl::instance_factory::impl
{

    bool compare(const char* lhs, const std::string& rhs)
    {
        return strcmp(lhs, rhs.c_str()) == 0;
    }

    std::vector<std::string> get_available_instance_extensions()
    {
        return ge::impl::factory::impl::extensions_names(vk::enumerateInstanceExtensionProperties());
    }

    std::vector<std::string> get_available_instance_layers()
    {
        return ge::impl::factory::impl::layers_names(vk::enumerateInstanceLayerProperties());
    }

} // namespace ge::impl::instance_factory::impl
