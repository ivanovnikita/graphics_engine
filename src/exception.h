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
        virtual ~exception() = default;
    };

    class vulkan_error : public ge::exception, public std::runtime_error
    {
    public:
        explicit vulkan_error(const std::string& what)
            : ge::exception      ()
            , std::runtime_error (what) {}
        explicit vulkan_error(const char* what)
            : ge::exception      ()
            , std::runtime_error (what) {}

        virtual ~vulkan_error() = default;

        virtual const char* what() const noexcept
        {
            return std::runtime_error::what();
        }
    };

    class device_capabilities_error : public vulkan_error
    {
    public:
        explicit device_capabilities_error(const std::string& what)
            : vulkan_error (what) {}
        explicit device_capabilities_error(const char* what)
            : vulkan_error (what) {}
    };

} // namespace ge
