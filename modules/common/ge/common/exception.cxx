module;

#include <string_view>

module exception;

namespace ge
{
    exception_base::exception_base
    (
        const std::string_view function_name,
        const size_t line_number,
        const std::string_view error_message,
        const std::string_view error_details
    ) noexcept
        : function_name_{function_name}
        , line_number_{line_number}
        , error_message_{error_message}
        , error_details_{error_details}
    {
    }

    exception_base::~exception_base() = default;

    const char* exception_base::what() const noexcept
    {
        return error_message_.data();
    }

    size_t exception_base::get_line_number() const noexcept
    {
        return line_number_;
    }

    std::string_view exception_base::get_function_name() const noexcept
    {
        return function_name_;
    }

    std::string_view exception_base::get_error_message() const noexcept
    {
        return error_message_;
    }

    std::string_view exception_base::get_error_details() const noexcept
    {
        return error_details_;
    }
}
