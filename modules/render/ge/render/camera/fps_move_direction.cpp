#include "fps_move_direction.h"

namespace ge
{
    FpsMoveDirection::FpsMoveDirection()
        : left_{0}
        , right_{0}
        , up_{0}
        , down_{0}
        , forward_{0}
        , backward_{0}
    {
    }

    void FpsMoveDirection::add(const FpsAction action)
    {
        switch (action)
        {
        case FpsAction::MoveCamera_Left:
        {
            ++left_;
            break;
        }
        case FpsAction::MoveCamera_Right:
        {
            ++right_;
            break;
        }
        case FpsAction::MoveCamera_Up:
        {
            ++up_;
            break;
        }
        case FpsAction::MoveCamera_Down:
        {
            ++down_;
            break;
        }
        case FpsAction::MoveCamera_Forward:
        {
            ++forward_;
            break;
        }
        case FpsAction::MoveCamera_Backward:
        {
            ++backward_;
            break;
        }
        default: break;
        }

        update();
    }

    void FpsMoveDirection::sub(const FpsAction action)
    {
        switch (action)
        {
        case FpsAction::MoveCamera_Left:
        {
            --left_;
            break;
        }
        case FpsAction::MoveCamera_Right:
        {
            --right_;
            break;
        }
        case FpsAction::MoveCamera_Up:
        {
            --up_;
            break;
        }
        case FpsAction::MoveCamera_Down:
        {
            --down_;
            break;
        }
        case FpsAction::MoveCamera_Forward:
        {
            --forward_;
            break;
        }
        case FpsAction::MoveCamera_Backward:
        {
            --backward_;
            break;
        }
        default: break;
        }

        update();
    }

    void FpsMoveDirection::update()
    {
        if (left_ > 0 and right_ == 0 and ((forward_ == 0 and backward_ == 0) or (forward_ > 0 and backward_ > 0)))
        {
            current_move_direction_ = FpsAction::MoveCamera_Left;
        }
        else if (right_ > 0 and left_ == 0 and ((forward_ == 0 and backward_ == 0) or (forward_ > 0 and backward_ > 0)))
        {
            current_move_direction_ = FpsAction::MoveCamera_Right;
        }
        else if (forward_ > 0 and backward_ == 0 and ((left_ == 0 and right_ == 0) or (left_ > 0 and right_ > 0)))
        {
            current_move_direction_ = FpsAction::MoveCamera_Forward;
        }
        else if (backward_ > 0 and forward_ == 0 and ((left_ == 0 and right_ == 0) or (left_ > 0 and right_ > 0)))
        {
            current_move_direction_ = FpsAction::MoveCamera_Backward;
        }
        else if (forward_ > 0 and left_ > 0 and backward_ == 0 and right_ == 0)
        {
            current_move_direction_ = FpsAction::MoveCamera_ForwardLeft;
        }
        else if (forward_ > 0 and right_ > 0 and backward_ == 0 and left_ == 0)
        {
            current_move_direction_ = FpsAction::MoveCamera_ForwardRight;
        }
        else if (backward_ > 0 and left_ > 0 and forward_ == 0 and right_ == 0)
        {
            current_move_direction_ = FpsAction::MoveCamera_BackwardLeft;
        }
        else if (backward_ > 0 and right_ > 0 and forward_ == 0 and left_ == 0)
        {
            current_move_direction_ = FpsAction::MoveCamera_BackwardRight;
        }
        else if (up_ > 0 and down_ == 0)
        {
            current_move_direction_ = FpsAction::MoveCamera_Up;
        }
        else if (down_ > 0 and up_ == 0)
        {
            current_move_direction_ = FpsAction::MoveCamera_Down;
        }
        else
        {
            current_move_direction_.reset();
        }
    }

    std::optional<FpsAction> FpsMoveDirection::current_move_direction()
    {
        return current_move_direction_;
    }
}
