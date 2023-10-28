#pragma once

#include <vector>
#include <span>

#include <cstdint>

namespace ge
{
    class Image final
    {
    public:
        explicit Image
        (
            size_t x,
            size_t y,
            size_t channels_count,
            std::span<const unsigned char>
        );

        size_t x() const noexcept;
        size_t y() const noexcept;
        size_t channels_count() const noexcept;

        size_t bytes_count() const noexcept;
        size_t pixels_count() const noexcept;

        unsigned char* data() noexcept;
        const unsigned char* data() const noexcept;

    private:
        size_t x_;
        size_t y_;
        size_t channels_count_;
        std::vector<unsigned char> data_;
    };
}
