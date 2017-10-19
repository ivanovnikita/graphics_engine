#pragma once

#include "exception.h"

#include <vector>

namespace ge::impl::factory::impl
{

    template<typename ContainerT, typename ContainerU, typename Comparator>
    auto not_contained_in
    (
        const ContainerT& what
      , const ContainerU& where
      , Comparator compare
    );

    template<typename Container>
    void all_required_are_available
    (
        const Container& required
      , const std::vector<std::string>& available
    );

    template<typename Container>
    std::vector<std::string> extensions_names(const Container&);

} // namespace ge::impl::factory::impl

#include "factory_tools_impl.hpp"
