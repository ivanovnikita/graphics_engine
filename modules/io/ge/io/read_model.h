#pragma once

#include "model.h"

#include <string>

namespace ge
{
    Model read_model_obj(const std::string& filepath);
}
