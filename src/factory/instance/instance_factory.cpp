#include "instance_factory.h"
#include "factory_tools.hpp"

namespace ge::impl::factory::instance
{

    namespace impl
    {
        std::vector<std::string> get_available_instance_extensions()
        {
            return ge::impl::factory::impl::extensions_names(vk::enumerateInstanceExtensionProperties());
        }

        std::vector<std::string> get_available_instance_layers()
        {
            return ge::impl::factory::impl::layers_names(vk::enumerateInstanceLayerProperties());
        }
    } // namespace impl

    vk::UniqueInstance create()
    {
        using namespace ge::impl::factory::impl;

        const vk::ApplicationInfo application_info
        {
            "graphics engine"
          , VK_MAKE_VERSION(1, 0, 0)
          , "no engine"
          , VK_MAKE_VERSION(1, 0, 0)
          , VK_MAKE_VERSION(1, 0, 0)
        };

        static constexpr const char* required_extensions[] =
        {
            VK_KHR_SURFACE_EXTENSION_NAME
          , VK_KHR_XCB_SURFACE_EXTENSION_NAME
          , VK_EXT_DEBUG_REPORT_EXTENSION_NAME
        };

        static constexpr const char* required_layers[] =
        {
            "VK_LAYER_LUNARG_standard_validation"
        };

        all_required_are_available(required_extensions, impl::get_available_instance_extensions());
        all_required_are_available(required_layers, impl::get_available_instance_layers());

        const vk::InstanceCreateInfo create_info
        {
            vk::InstanceCreateFlags()
          , &application_info
          , static_cast<uint32_t>(std::size(required_layers))
          , std::data(required_layers)
          , static_cast<uint32_t>(std::size(required_extensions))
          , std::data(required_extensions)
        };

        return vk::createInstanceUnique(create_info);
    }

} // namespace ge::impl::factory::instance
