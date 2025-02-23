#include "task.hpp"

#include "entity/attach-entities/crewmate.hpp"
#include "entity/root-entities/ship.hpp"

void ActionMove::act(Crewmate& crewmate, sf::Time deltaTime)
{
    crewmate.move(direction.normalized() * std::min(crewmate.speed * deltaTime.asSeconds(), direction.length()));
}

std::variant<Action, TaskError, TaskDone> MoveTask::resolve(const Crewmate& crewmate)
{
    constexpr float stepEpsilon = 0.1f;

    const auto ship = dynamic_cast<Ship*>(crewmate.parent);
    const auto currentPos = crewmate.getPosition();

    if ((m_targetPos - currentPos).length() <= stepEpsilon)
    {
        return TaskDone{};
    }

    if (m_steps.empty())
    {
        const Location gridLocation = crewmate.posToGridLocation(currentPos, Ship::blockSize);
        const Location targetLocation = crewmate.posToGridLocation(m_targetPos, Ship::blockSize);
        m_steps = ship->pathfind(gridLocation, targetLocation);
    }

    if (m_steps.empty())
    {
        return TaskErrorCouldNotReach{};
    }

    auto currentStepPosition = ship->locationToPosition(m_steps[0]) + Ship::blockSize / 2.f;
    if ((currentStepPosition - currentPos).length() <= stepEpsilon)
    {
        m_steps.erase(m_steps.begin());
        if (m_steps.empty())
        {
            return TaskDone{};
        }

        currentStepPosition = ship->locationToPosition(m_steps[0]) + Ship::blockSize / 2.f;
    }

    const auto dir = currentStepPosition - currentPos;
    return ActionMove{dir};
}
