#pragma once

#include <stdexcept>

namespace ge
{

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

    class vulkan_error : public ge::exception, public std::runtime_error
    {
    public:
        explicit vulkan_error(const std::string& what);
        explicit vulkan_error(const char* what);
        virtual ~vulkan_error();
        virtual const char* what() const noexcept;
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
        virtual ~window_error();
        virtual const char* what() const noexcept;
    };

    class overflow_error : public ge::exception, public std::overflow_error
    {
    public:
        using std::overflow_error::overflow_error;
        virtual ~overflow_error();
        virtual const char* what() const noexcept;
    };

} // namespace ge
