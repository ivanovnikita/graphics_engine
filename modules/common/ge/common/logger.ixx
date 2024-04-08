export module logger;

import flags;
import log;

namespace ge
{
    export enum class LogType
    {
        Error = 0,
        ErrorDetails = 1,
        Warning = 2,
        SystemInfo = 3
    };

    export class Logger
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
