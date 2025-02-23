#pragma once

#include "block.hpp"
#include "units.hpp"

#include <SFML/System/Time.hpp>

#include <memory>
#include <optional>
#include <utility>
#include <variant>
#include <vector>

class Crewmate;

struct ActionMove
{
    Position direction{};

    void act(Crewmate& crewmate, sf::Time deltaTime);
};

struct ActionTakeItem
{
    void act(Crewmate& crewmate, sf::Time deltaTime)
    {
    }
};

using Action = std::variant<ActionMove, ActionTakeItem>;

struct TaskErrorCouldNotReach
{
};

using TaskError = std::variant<TaskErrorCouldNotReach>;

struct TaskDone
{
};

class Task
{
public:
    virtual std::optional<TaskError> onStart(const Crewmate& crewmate)
    {
        return {};
    }
    virtual std::variant<Action, TaskError, TaskDone> resolve(const Crewmate& crewmate) = 0;
    virtual void onCancel(const Crewmate& crewmate){

    }
};

class MoveTask : public Task
{
public:
    MoveTask(Position targetPos) : m_targetPos{targetPos}
    {
    }

private:
    std::variant<Action, TaskError, TaskDone> resolve(const Crewmate& crewmate) override;

    Position m_targetPos;
    std::vector<Location> m_steps;
};