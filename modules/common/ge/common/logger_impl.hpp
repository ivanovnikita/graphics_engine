#pragma once

#include "logger.hpp"
#include "log.hpp"

namespace ge
{
    template <typename... Args>
    void Logger::log(const LogType log_type, const Args&... args) const noexcept
    {
        if (enabled(log_type))
        {
            ::ge::log(LogDestination::StdOut, args...);
            log_flush(LogDestination::StdOut);
        }
    }
}
