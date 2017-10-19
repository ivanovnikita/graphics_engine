#pragma once

#include "instance_factory.hpp"
#include "factory_tools.hpp"

namespace ge::impl::factory::instance
{

    namespace impl
    {
        std::vector<std::string> get_available_instance_extensions();
        std::vector<std::string> get_available_instance_layers();
    } // namespace impl


    template<typename Extensions, typename Layers>
    vk::UniqueInstance create
    (
        const Extensions& required_extensions
      , const Layers& required_layers
    )
    {
        using namespace ge::impl::factory::impl;
        using ExtensionsType = std::remove_pointer_t<decltype(std::data(required_extensions))>;
        using LayersType = std::remove_pointer_t<decltype(std::data(required_layers))>;

        static_assert(std::is_same_v<std::remove_const_t<std::remove_pointer_t<ExtensionsType>>, char>
                      && std::is_pointer_v<ExtensionsType>, "Extensions names must be saved as char*");
        static_assert(std::is_same_v<std::remove_const_t<std::remove_pointer_t<LayersType>>, char>
                      && std::is_pointer_v<LayersType>, "Layers names must be saved as char*");

        const vk::ApplicationInfo application_info
        {
              "graphics engine"
            , VK_MAKE_VERSION(1, 0, 0)
            , "no engine"
            , VK_MAKE_VERSION(1, 0, 0)
            , VK_MAKE_VERSION(1, 0, 0)
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
