#include "read_image.h"
#include "ge/common/exception.h"
#include "ge/common/safe_cast.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP
#include <stb_image.h>

namespace ge
{
    Image read_image(const std::string& filepath)
    {
        int x = 0;
        int y = 0;
        int channels_in_file = 0;
        constexpr int desired_channels = 0;

        unsigned char* const data = stbi_load
        (
            filepath.c_str(),
            &x,
            &y,
            &channels_in_file,
            desired_channels
        );

        if (data == nullptr)
        {
            GE_THROW_EXPECTED_ERROR("Image loading failed");
        }

        try
        {
            Image image
            {
                safe_cast<size_t>(x),
                safe_cast<size_t>(y),
                safe_cast<size_t>(channels_in_file),
                {data, safe_cast<size_t>(x * y * channels_in_file)}
            };

            stbi_image_free(data);

            return image;

        }
        catch (const expected_error&)
        {
            stbi_image_free(data);
            throw;
        }
    }
}
