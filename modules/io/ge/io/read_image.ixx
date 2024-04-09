module;

#include <string>

export module read_image;

export import image;

namespace ge
{
    export Image read_image(const std::string& filepath);
}
