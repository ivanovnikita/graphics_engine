#include "ge/render/render_i.h"

namespace ge
{
    DrawableI::~DrawableI() = default;
    WithCamera2dI::~WithCamera2dI() = default;
    WithCamera3dI::~WithCamera3dI() = default;
    Render2dI::~Render2dI() = default;
    Render3dI::~Render3dI() = default;

    NeedRedraw combine_need_redraw(const NeedRedraw old_value, const NeedRedraw new_value)
    {
        return static_cast<NeedRedraw>(static_cast<uint8_t>(old_value) | static_cast<uint8_t>(new_value));
    }
}
