#pragma once

#include "exception.h"
#include "options.h"

#include <vector>

namespace ge::impl::factory::tools
{

    template<typename ContainerT, typename ContainerU>
    auto not_contained_in
    (
        const ContainerT& what
      , const ContainerU& where
    );

    template<typename ContainerT, typename ContainerU>
    void all_required_are_available
    (
        const ContainerT& required
      , const ContainerU& available
    );

    template<typename Container>
    std::vector<std::string> extensions_names(const Container& extensions);

    template<typename Container>
    std::vector<std::string> layers_names(const Container& layers);

    std::vector<const char*> get_required_layers(const options::ValidationLayers&);

}

#include "factory/tools_impl.hpp"
