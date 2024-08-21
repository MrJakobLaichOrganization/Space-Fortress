#include "crewmate.hpp"

#include "world.hpp"
#include "pathfinding.hpp"
#include "entity/root-entities/ship.hpp"

#include <iostream>

Crewmate::Crewmate(World* world, Id id, std::string_view name, Gender gender) :
    AttachEntity{world, id},
    m_name(name),
    m_gender(gender)
{
    m_birthTimestamp = world->getTime();
}

void Crewmate::update(sf::Time deltaTime)
{
    BlockGrid::Location gridLocation = posToGridLocation(getPosition(), static_cast<sf::Vector2u>(Ship::blockSize));

    if (m_targetDest != gridLocation && !m_steps.size())
    {
        m_steps = dynamic_cast<Ship*>(parent)->pathfind(gridLocation, m_targetDest);
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
