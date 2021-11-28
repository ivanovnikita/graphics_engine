#pragma once

#include <vulkan/vulkan.hpp>

#include <stdexcept>
#include <string_view>

namespace ge
{

    #define GE_THROW(exception, message)                            \
        throw exception(std::string("file: ") + __FILE__ + "\n" +   \
        "function: " + __func__ + "\n" +                            \
        "line: " + std::to_string(__LINE__) + "\n" +                \
        "message: " + message)

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

    std::string_view to_string_view(const vk::Result& result) noexcept;

#define GE_THROW_ERROR(exception, message) \
    throw exception(__func__, __LINE__, message)

#define GE_THROW_EXPECTED_ERROR(message) \
    GE_THROW_ERROR(expected_error, message)

#define GE_THROW_UNEXPECTED_ERROR(message) \
    GE_THROW_ERROR(unexpected_error, message)

#define GE_THROW_EXPECTED_RESULT(vk_result) \
    GE_THROW_ERROR(expected_error, to_string_view(vk_result))

#define GE_THROW_UNEXPECTED_RESULT(vk_result) \
    GE_THROW_ERROR(unexpected_error, to_string_view(vk_result))

    class exception
    {
    public:
        virtual ~exception();
        virtual const char* what() const noexcept = 0;
    };

    class vulkan_error : public ::ge::exception, public std::runtime_error
    {
    public:
        explicit vulkan_error(const std::string& what);
        explicit vulkan_error(const char* what);
        virtual ~vulkan_error() override;
        virtual const char* what() const noexcept override;
    };

    class device_capabilities_error : public vulkan_error
    {
    public:
        explicit device_capabilities_error(const std::string& what);
        explicit device_capabilities_error(const char* what);
        ~device_capabilities_error() override;
    };

    class window_error : public ge::exception, public std::runtime_error
    {
    public:
        explicit window_error(const std::string& what);
        explicit window_error(const char* what);
        virtual ~window_error() override;
        virtual const char* what() const noexcept override;
    };

    class overflow_error : public ge::exception, public std::overflow_error
    {
    public:
        using std::overflow_error::overflow_error;
        virtual ~overflow_error() override;
        virtual const char* what() const noexcept override;
    };

    class invalid_options : public ge::exception, public std::logic_error
    {
    public:
        using std::logic_error::logic_error;
        virtual ~invalid_options() override;
        virtual const char* what() const noexcept override;
    };

    class invalid_shader : public ge::exception, public std::logic_error
    {
    public:
        using std::logic_error::logic_error;
        virtual ~invalid_shader() override;
        virtual const char* what() const noexcept override;
    };

}
