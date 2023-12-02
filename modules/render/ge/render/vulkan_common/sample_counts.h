#pragma once

#include "device.h"

#include "ge/render/antialiasing/msaa.h"

#include "ge/common/flags.hpp"

namespace ge
{
    Flags<MsaaSamples> get_sample_counts(const PhysicalDeviceData&);
}
