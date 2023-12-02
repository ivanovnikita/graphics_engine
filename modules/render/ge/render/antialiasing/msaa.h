#pragma once

#include "ge/common/flags.hpp"
#include "ge/common/logger.hpp"

#include <optional>

namespace ge
{
    enum class MsaaSamples
    {
        x2 = 2,
        x4 = 4,
        x8 = 8
    };

    std::optional<MsaaSamples> choose_max
    (
        MsaaSamples desired,
        const Flags<MsaaSamples>& available,
        const Logger&
    );
}
