#pragma once

#include "fps_action.h"

#include <optional>

#include <cstdint>

namespace ge
{
    class FpsMoveDirection final
    {
    public:
        explicit FpsMoveDirection();

        void add(FpsAction);
        void sub(FpsAction);

        std::optional<FpsAction> current_move_direction();

    private:
        void update();

        uint8_t left_;
        uint8_t right_;
        uint8_t up_;
        uint8_t down_;
        uint8_t forward_;
        uint8_t backward_;

        std::optional<FpsAction> current_move_direction_;
    };
}
