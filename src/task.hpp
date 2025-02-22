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
    Location position;
    TaskType type = TaskType::None;

    bool operator==(const Task& other) const
    {
        return position == other.position && type == other.type;
    }
};