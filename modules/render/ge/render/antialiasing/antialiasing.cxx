module;

#include <optional>

module antialiasing;

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
