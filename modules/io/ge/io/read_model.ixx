module;

#include <string>

export module read_model;

export import model;

namespace ge
{
    export Model read_model_obj(const std::string& filepath);
}
