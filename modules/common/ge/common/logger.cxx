module logger;

namespace ge
{
    Logger::Logger(const Flags<LogType> enabled_logs) noexcept
        : enabled_logs_{enabled_logs}
    {
    }

    bool Logger::enabled(const LogType log_type) const noexcept
    {
        return enabled_logs_.test(log_type);
    }
}
