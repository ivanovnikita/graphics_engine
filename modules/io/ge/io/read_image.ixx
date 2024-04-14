module;

#include <string>

export module io.read_image;

export import io.image;

namespace ge
{
    export Image read_image(const std::string& filepath);
}
