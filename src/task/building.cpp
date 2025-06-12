#include "building.hpp"

#include "entity/attach-entities/crewmate.hpp"
#include "entity/root-entities/ship.hpp"

ActPtr BuildingTask::start()
{
    const auto targetPosition = Ship::locationToPosition(location) + Ship::blockSize / 2.f;

    return std::make_unique<ActSequence>(std::make_unique<MoveAct>(targetPosition, Ship::blockSize.x),
                                         std::make_unique<BuildingAct>(location, duration));
}

BuildingAct::BuildingAct(Location location, float duration) : location{location}, duration{duration}
{
}

Act::Status BuildingAct::doAct(Crewmate& crewmate, sf::Time time)
{
    duration -= time.asSeconds();
    if (duration <= 0.f)
    {
        auto ship = dynamic_cast<Ship*>(crewmate.parent);
        ship->grid.setBlockType("Wall", location);

        return Status::Success;
    }

    return Status::Running;
}
