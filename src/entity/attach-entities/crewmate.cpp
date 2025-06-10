#include "crewmate.hpp"

#include "entity/root-entities/ship.hpp"
#include "pathfinding.hpp"
#include "task/task.hpp"
#include "world.hpp"

#include <iostream>

Crewmate::Crewmate(World* world, Id id, std::string_view name, Gender gender) :
    AttachEntity{world, id},
    m_name(name),
    m_gender(gender)
{
    m_birthTimestamp = world->getTime();
    localBounds = {sf::Vector2f{-16.f, -16.f}, sf::Vector2f{32.f, 32.f}};
}
Crewmate::~Crewmate()
{
    if (currentTask)
    {
        currentTask->worker = nullptr;
        if (auto ship = dynamic_cast<Ship*>(parent))
        {
            ship->removeTask(*currentTask);
        }
    }
}

void Crewmate::update(sf::Time deltaTime) // NOLINT
{
    Ship& ship = *dynamic_cast<Ship*>(parent);

    if (!currentTask)
    {
        if (!ship.takeTask(*this))
        {
            return;
        }
    }

    if (!currentAct)
    {
        currentAct = currentTask->start();
    }

    if (!currentAct)
    {
        assert(false);
        return;
    }

    auto actStatus = currentAct->doAct(*this, deltaTime);
    if (actStatus == Act::Status::Fail || actStatus == Act::Status::Success)
    {
        if (currentTask && currentTask->removeOnSuccess)
        {
            ship.removeTask(*currentTask);
        }

        currentAct = nullptr;
    }
}

Time Crewmate::getAge() const
{
    return world->getTime() - m_birthTimestamp;
}

bool Crewmate::isAdult() const
{
    return getAge().year() >= adultYear;
}
