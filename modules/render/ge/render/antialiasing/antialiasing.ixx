module;

#include <optional>
#include <variant>

#include <cstdint>

export module antialiasing;

export import msaa;
export import logger;

namespace ge
{
    export struct NoAntialiasing final
    {
    };

    export struct Msaa final
    {
        MsaaSamples samples;
        bool enable_sample_shading;
    };

    export using Antialiasing = std::variant
    <
        NoAntialiasing,
        Msaa
    >;

    export Antialiasing convert_to_max_available
    (
        const Msaa&,
        const Flags<MsaaSamples>& available,
        const Logger&
    );
}

module : private;

namespace ge
{
    Antialiasing convert_to_max_available
    (
        const Msaa& msaa,
        const Flags<MsaaSamples>& available,
        const Logger& logger
    )
    {
        const std::optional<MsaaSamples> chosen = choose_max
        (
            msaa.samples,
            available,
            logger
        );

        if (chosen.has_value())
        {
            Msaa result = msaa;
            result.samples = *chosen;
            return result;
        }

        return NoAntialiasing{};
    }
}
