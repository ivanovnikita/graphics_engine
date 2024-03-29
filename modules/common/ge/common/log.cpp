#include "log.hpp"

namespace ge
{
    namespace detail
    {
        namespace
        {
            void log(FILE& file, const std::string_view str) noexcept
            {
                fwrite
                (
                    str.data(),
                    sizeof(std::string_view::value_type),
                    str.size(),
                    &file
                );
            }

            void log_flush(FILE& file) noexcept
            {
                fflush(&file);
            }
        }

        void log(const LogDestination destination, const std::string_view str) noexcept
        {
            switch (destination)
            {
            case LogDestination::StdOut:
            {
                log(*stdout, str);
                break;
            }
            case LogDestination::StdErr:
            {
                log(*stderr, str);
                break;
            }
            }
        }
    }

    void log_flush(const LogDestination destination) noexcept
    {
        switch (destination)
        {
        case LogDestination::StdOut:
        {
            detail::log_flush(*stdout);
            break;
        }
        case LogDestination::StdErr:
        {
            detail::log_flush(*stderr);
            break;
        }
        }
    }
}
