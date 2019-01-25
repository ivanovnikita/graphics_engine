#pragma once

#include "ge/render/factory/tools.hpp"
#include "ge/render/exception.h"

#include <vulkan/vulkan.hpp>

#include <string>
#include <algorithm>

namespace ge::factory
{
    inline bool operator==(const char* lhs, const std::string& rhs)
    {
        return strcmp(lhs, rhs.c_str()) == 0;
    }

    template<typename ContainerT, typename ContainerU>
    auto not_contained_in
    (
        const ContainerT& what
      , const ContainerU& where
    )
    {
        using T = std::remove_const_t<std::remove_pointer_t<decltype(std::data(what))>>;

        std::vector<T> absent;
        std::for_each
        (
            std::begin(what)
          , std::end(what)
          , [&where, &absent] (const auto& required)
          {
              if (std::none_of
              (
                  std::begin(where)
                , std::end(where)
                , [&required](const auto& available)
                {
                    return required == available;
                }
              ))
              {
                  absent.emplace_back(required);
              }
          }
        );
        return absent;
    }

    template<typename ContainerT, typename ContainerU>
    void all_required_are_available
    (
        const ContainerT& required
      , const ContainerU& available
    )
    {
        if (std::empty(required))
        {
            return;
        }

        const auto absent = not_contained_in
        (
            required
          , available
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

    inline auto get_extension_name(const vk::ExtensionProperties& property)
    {
        return property.extensionName;
    }

    inline auto get_layer_name(const vk::LayerProperties& property)
    {
        return property.layerName;
    }

    template<typename Container, typename NameGetter>
    std::vector<std::string> properties_names(const Container& extensions, NameGetter get_name)
    {
        std::vector<std::string> result;
        result.reserve(extensions.size());
        for (const auto& extension : extensions)
        {
            result.emplace_back(get_name(extension));
        }
        return result;
    }

    template<typename Container>
    std::vector<std::string> extensions_names(const Container& extensions)
    {
        return properties_names(extensions, get_extension_name);
    }

    template<typename Container>
    std::vector<std::string> layers_names(const Container& layers)
    {
        return properties_names(layers, get_layer_name);
    }
}
