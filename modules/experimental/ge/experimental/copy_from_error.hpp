#pragma once

#include "concepts.hpp"
#include "errors.hpp"

#include <type_traits>
#include <tuple>

namespace ge
{
    namespace detail
    {
        template <typename... T>
        struct CopyFromError;

        template <typename... Ts>
        struct CopyFromError<std::tuple<Ts...>> final
        {
            using Error = MergeUnique
            <
                typename std::invoke_result_t<decltype(Ts::copy_from), const Ts&>::Err...
            >;
        };
    }

    template <typename... Ts>
    using CopyFromError = typename detail::CopyFromError<Filter<CopyableFromT, std::tuple<Ts...>>>::Error;
}
