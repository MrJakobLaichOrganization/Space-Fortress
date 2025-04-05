#include "task.hpp"

#include "entity/attach-entities/crewmate.hpp"
#include "entity/root-entities/ship.hpp"

Act::Status MoveAct::doAct(Crewmate& crewmate, sf::Time deltaTime)
{
    constexpr float stepEpsilon = 0.1f;

    const auto ship = dynamic_cast<Ship*>(crewmate.parent);
    const auto currentPos = crewmate.getPosition();

    if ((targetPos - currentPos).length() <= stepEpsilon)
    {
        return Status::Success;
    }

    if (steps.empty())
    {
        const Location gridLocation = crewmate.posToGridLocation(currentPos, Ship::blockSize);
        const Location targetLocation = crewmate.posToGridLocation(targetPos, Ship::blockSize);
        steps = ship->pathfind(gridLocation, targetLocation);
    }

    if (steps.empty())
    {
        return Status::Fail;
    }

    auto currentStepPosition = ship->locationToPosition(steps[0]) + Ship::blockSize / 2.f;
    if ((currentStepPosition - currentPos).length() <= stepEpsilon)
    {
        steps.erase(steps.begin());
        if (steps.empty())
        {
            return Status::Success;
        }

        currentStepPosition = ship->locationToPosition(steps[0]) + Ship::blockSize / 2.f;
    }

    const auto dir = currentStepPosition - currentPos;
    crewmate.move(dir.normalized() * std::min(crewmate.speed * deltaTime.asSeconds(), dir.length()));
    return Status::Running;
}
