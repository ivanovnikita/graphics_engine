#pragma once

#include "instance_factory.hpp"

namespace ge::impl::instance_factory
{

    namespace impl
    {

        bool compare(const char* lhs, const std::string& rhs);
        std::vector<std::string> get_available_instance_extensions();
        std::vector<std::string> get_available_instance_layers();

        template<typename ContainerT, typename ContainerU, typename Comparator>
        auto not_contained_in
        (
            const ContainerT& what
          , const ContainerU& where
          , Comparator compare
        )
        {
            using T = std::remove_const_t<std::remove_pointer_t<decltype(std::data(what))>>;

            std::vector<T> absent;
            std::for_each
            (
                std::begin(what)
              , std::end(what)
              , [&where, &compare, &absent] (const auto& required)
              {
                  if (std::none_of
                  (
                      std::begin(where)
                    , std::end(where)
                    , [&required, &compare](const auto& available)
                    {
                        return compare(required, available);
                    }
                  ))
                  {
                      absent.emplace_back(required);
                  }
              }
            );
            return absent;
        }

        template<typename Container>
        void all_required_are_available
        (
            const Container& required
          , const std::vector<std::string>& available
        )
        {
            const auto absent = not_contained_in
            (
                required
              , available
              , compare
            );

            if (!absent.empty())
            {
                std::string message("These features are not available: \n");
                for (const auto& name : absent)
                {
                    message.append(name);
                    message.append("\n");
                }
                GE_THROW(device_capabilities_error, message);
            }
        }

    } // namespace impl


    template<typename Extensions, typename Layers>
    vk::UniqueInstance create
    (
        const Extensions& required_extensions
      , const Layers& required_layers
    )
    {
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

        impl::all_required_are_available(required_extensions, impl::get_available_instance_extensions());
        impl::all_required_are_available(required_layers, impl::get_available_instance_layers());

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

} // namespace ge::impl::instance_factory
