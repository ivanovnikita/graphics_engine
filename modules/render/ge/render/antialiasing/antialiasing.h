#pragma once

#include "msaa.h"
#include "ge/common/logger.hpp"

#include <variant>

#include <cstdint>

namespace ge
{
    struct NoAntialiasing final
    {
    };

    struct Msaa final
    {
        MsaaSamples samples;
        bool enable_sample_shading;
    };

    using Antialiasing = std::variant
    <
        NoAntialiasing,
        Msaa
    >;

    Antialiasing convert_to_max_available
    (
        const Msaa&,
        const Flags<MsaaSamples>& available,
        const Logger&
    );
}
