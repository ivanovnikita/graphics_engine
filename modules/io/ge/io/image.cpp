#include "image.h"
#include "ge/common/exception.h"

#include <cassert>
#include <cstring>

namespace ge
{
    Image::Image
    (
        const size_t x,
        const size_t y,
        const size_t channels_count,
        const std::span<const unsigned char> data
    )
        : x_{x}
        , y_{y}
        , channels_count_{channels_count}
    {
        const size_t bytes_count = x * y * channels_count;
        assert(data.size() == bytes_count);

        try
        {
            data_.resize(bytes_count);
        }
        catch (const std::bad_alloc&)
        {
            GE_THROW_EXPECTED_ERROR("Allocation for image failed");
        }

        std::memcpy(data_.data(), data.data(), bytes_count);
    }

    size_t Image::x() const noexcept
    {
        return x_;
    }

    size_t Image::y() const noexcept
    {
        return y_;
    }

    size_t Image::channels_count() const noexcept
    {
        return channels_count_;
    }

    size_t Image::bytes_count() const noexcept
    {
        return pixels_count() * channels_count_;
    }

    size_t Image::pixels_count() const noexcept
    {
        return x_ * y_;
    }

    unsigned char* Image::data() noexcept
    {
        return data_.data();
    }

    const unsigned char* Image::data() const noexcept
    {
        return data_.data();
    }
}
