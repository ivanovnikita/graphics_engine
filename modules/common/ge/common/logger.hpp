#pragma once

#include "flags.hpp"

namespace ge
{
    enum class LogType
    {
        Error = 0,
        ErrorDetails = 1,
        SystemInfo = 2
    };

    class Logger
    {
    public:
        explicit Logger(Flags<LogType> enabled_logs) noexcept;

        bool enabled(LogType) const noexcept;

        template <typename... Args>
        void log(LogType, const Args&...) const noexcept;


    private:
        Flags<LogType> enabled_logs_;
    };
}

#include "logger_impl.hpp"
