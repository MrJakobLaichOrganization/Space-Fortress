#include "crewmate.hpp"

#include "world.hpp"
#include "pathfinding.hpp"

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
    if (m_movementGrid)
    {
        // TODO: Remove the magic 64 number, we need to be able to fetch rendering dimensions
        // Proposal: the grid class should be converting the pos to grid location
        BlockGrid::Location gridLocation = posToGridLocation(getPosition(), {64, 64});

        if (m_targetDest != gridLocation && !m_steps.size())
        {
            m_steps = generatePath(*m_movementGrid, gridLocation, m_targetDest);
        }
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
