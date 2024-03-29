#include "msaa.h"

#include <array>

namespace ge
{
    namespace
    {
        std::string_view to_string_view(const MsaaSamples value)
        {
            switch (value)
            {
            case MsaaSamples::x8: return "x8";
            case MsaaSamples::x4: return "x4";
            case MsaaSamples::x2: return "x2";
            }
            __builtin_unreachable();
        }
    }

    std::optional<MsaaSamples> choose_max
    (
        const MsaaSamples desired,
        const Flags<MsaaSamples>& available,
        const Logger& logger
    )
    {
        if (available.test(desired))
        {
            return desired;
        }

        constexpr std::array all
        {
            MsaaSamples::x8,
            MsaaSamples::x4,
            MsaaSamples::x2
        };

        auto it = std::find(all.cbegin(), all.cend(), desired);
        assert(it != all.cend());

        while (it != all.cend())
        {
            if (available.test(*it))
            {
                if (logger.enabled(LogType::Warning))
                {
                    logger.log
                    (
                        LogType::Warning,
                        "Selected msaa (",
                        to_string_view(*it),
                        ") is lower than desired (",
                        to_string_view(desired),
                        ")"
                    );
                }
                return *it;
            }

            ++it;
        }

        return std::nullopt;
    }
}
