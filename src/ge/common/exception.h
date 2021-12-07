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
            const std::string_view error_message
        ) noexcept
            : function_name_{function_name}
            , line_number_{line_number}
            , error_message_{error_message}
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

    private:
        std::string_view function_name_;
        size_t line_number_;
        std::string_view error_message_;
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

#define GE_THROW_ERROR(exception, message) \
    throw exception(__func__, __LINE__, message)

#define GE_THROW_EXPECTED_ERROR(message) \
    GE_THROW_ERROR(expected_error, message)

#define GE_THROW_UNEXPECTED_ERROR(message) \
    GE_THROW_ERROR(unexpected_error, message)

}
