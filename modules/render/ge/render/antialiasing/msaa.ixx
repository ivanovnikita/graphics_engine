module;

#include <optional>

export module msaa;

export import flags;
export import logger;

namespace ge
{
    export enum class MsaaSamples
    {
        x2 = 2,
        x4 = 4,
        x8 = 8
    };

    export std::optional<MsaaSamples> choose_max
    (
        MsaaSamples desired,
        const Flags<MsaaSamples>& available,
        const Logger&
    );
}
