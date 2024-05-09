#include "camera_3d_mover_strategy.h"

namespace ge
{
    Camera3dMoverStrategy::Camera3dMoverStrategy(WithCamera3dI& render)
        : Camera3dMoverI{render}
    {
    }

    Camera3dMoverStrategy::~Camera3dMoverStrategy() = default;

    // TODO: добавить сдвиг камеры движением мыши в сторону края экрана, как в стратегиях
}
