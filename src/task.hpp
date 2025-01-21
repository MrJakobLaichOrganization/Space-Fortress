#pragma once

#include "block.hpp"

enum class TaskType : std::uint8_t
{
    None,
    Idle,
    Work,
    Build,
    Destroy
};

struct Task
{
    BlockGrid::Location position;
    TaskType type = TaskType::None;
};