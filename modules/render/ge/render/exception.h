#pragma once

#include "ge/common/exception.h"
#include "to_string_view_enum.h"

#include <vulkan/vulkan.hpp>

#include <stdexcept>
#include <string_view>

namespace ge
{
#define GE_THROW_EXPECTED_RESULT(vk_result, message) \
    GE_THROW_ERROR(expected_error, message, vk::to_string_view(vk_result))

#define GE_THROW_UNEXPECTED_RESULT(vk_result, message) \
    GE_THROW_ERROR(unexpected_error, message, vk::to_string_view(vk_result))

    #define GE_THROW(exception, message)                            \
        throw exception(std::string("file: ") + __FILE__ + "\n" +   \
        "function: " + __func__ + "\n" +                            \
        "line: " + std::to_string(__LINE__) + "\n" +                \
        "message: " + message)

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
