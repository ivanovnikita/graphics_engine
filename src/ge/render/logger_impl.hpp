#pragma once

#include "logger.hpp"

#include <cstdio>

namespace ge
{
    template <typename... Args>
    void Logger::log(const LogType log_type, const char* fmt, const Args&... args) const noexcept
    {
        if (enabled(log_type))
        {
            std::fprintf(stdout, fmt, args...);
        }
    }
}
