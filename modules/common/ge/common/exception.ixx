module;

#include <string_view>

export module exception;

namespace ge
{
    export class exception_base : public std::exception
    {
    public:
        exception_base
        (
            const std::string_view function_name,
            const size_t line_number,
            const std::string_view error_message,
            const std::string_view error_details = ""
        ) noexcept;

        ~exception_base() override;

        const char* what() const noexcept override;

        size_t get_line_number() const noexcept;
        std::string_view get_function_name() const noexcept;
        std::string_view get_error_message() const noexcept;
        std::string_view get_error_details() const noexcept;

    private:
        std::string_view function_name_;
        size_t line_number_;
        std::string_view error_message_;
        std::string_view error_details_;
    };

    export class expected_error : public exception_base
    {
    public:
        using exception_base::exception_base;
    };

    export class unexpected_error : public exception_base
    {
    public:
        using exception_base::exception_base;
    };
}
