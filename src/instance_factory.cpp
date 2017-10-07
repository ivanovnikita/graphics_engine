#include "instance_factory.hpp"

namespace ge::impl::instance_factory::impl
{

    bool compare(const char* lhs, const std::string& rhs)
    {
        return strcmp(lhs, rhs.c_str()) == 0;
    }

    std::vector<std::string> get_available_instance_extensions()
    {
        const auto extension_properties = vk::enumerateInstanceExtensionProperties();

        std::vector<std::string> result;
        result.reserve(extension_properties.size());
        for (const auto& extension : extension_properties)
        {
            result.emplace_back(std::move(extension.extensionName));
        }

        return result;
    }

    std::vector<std::string> get_available_instance_layers()
    {
        const auto layer_properties = vk::enumerateInstanceLayerProperties();

        std::vector<std::string> result;
        result.reserve(layer_properties.size());
        for (const auto& layer : layer_properties)
        {
            result.emplace_back(std::move(layer.layerName));
        }

        return result;
    }

} // namespace ge::impl::instance_factory::impl
