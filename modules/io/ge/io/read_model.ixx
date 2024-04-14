module;

#include <string>

export module io.read_model;

export import io.model;

namespace ge
{
    export Model read_model_obj(const std::string& filepath);
}
