#include "camera_3d_mover_i.h"

#include <iostream>

namespace ge
{
    Camera3dMoverI::Camera3dMoverI(WithCamera3dI& render)
        : render_{render}
    {
    }

    Camera3dMoverI::~Camera3dMoverI() = default;
}
