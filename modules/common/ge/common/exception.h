#pragma once

#include <stdexcept>
#include <string_view>

namespace ge
{
    class exception_base : public std::exception
    {
    public:
        exception_base
        (
            const std::string_view function_name,
            const size_t line_number,
            const std::string_view error_message,
            const std::string_view error_details = ""
        ) noexcept
            : function_name_{function_name}
            , line_number_{line_number}
            , error_message_{error_message}
            , error_details_{error_details}
        {
        }

        ~exception_base() override = default;

        const char* what() const noexcept override
        {
            return error_message_.data();
        }

        size_t get_line_number() const noexcept
        {
            return line_number_;
        }

        std::string_view get_function_name() const noexcept
        {
            return function_name_;
        }

        std::string_view get_error_message() const noexcept
        {
            return error_message_;
        }

        std::string_view get_error_details() const noexcept
        {
            return error_details_;
        }

    private:
        std::string_view function_name_;
        size_t line_number_;
        std::string_view error_message_;
        std::string_view error_details_;
    };

    class expected_error : public exception_base
    {
    public:
        using exception_base::exception_base;
    };

    class unexpected_error : public exception_base
    {
    public:
        using exception_base::exception_base;
    };

#define GE_THROW_ERROR(exception, ...) \
    throw exception(__func__, __LINE__, __VA_ARGS__)

#define GE_THROW_EXPECTED_ERROR(...) \
    GE_THROW_ERROR(expected_error, __VA_ARGS__)

#define GE_THROW_UNEXPECTED_ERROR(...) \
    GE_THROW_ERROR(unexpected_error, __VA_ARGS__)

}
