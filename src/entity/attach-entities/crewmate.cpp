#include "crewmate.hpp"

#include "entity/root-entities/ship.hpp"
#include "pathfinding.hpp"
#include "world.hpp"

#include <iostream>

Crewmate::Crewmate(World* world, Id id, std::string_view name, Gender gender) :
    AttachEntity{world, id},
    m_name(name),
    m_gender(gender)
{
    m_birthTimestamp = world->getTime();
}

void Crewmate::update(sf::Time deltaTime) // NOLINT
{
    step(deltaTime);
}

void Crewmate::step(sf::Time deltaTime)
{
    const BlockGrid::Location gridLocation = posToGridLocation(getPosition(), static_cast<sf::Vector2u>(Ship::blockSize));

    if (targetLocation == gridLocation)
    {
        return;
    }

    if (m_steps.empty())
    {
        updatePathfinding();
    }

    if (m_steps.size() < 2)
    {
        return;
    }

    auto currentStep = m_steps[0];
    auto nextStep = m_steps[1];
    if (nextStep == gridLocation)
    {
        m_steps.erase(m_steps.begin());

        currentStep = m_steps[0];
        nextStep = m_steps[1];
    }

    if (currentStep != gridLocation)
    {
        updatePathfinding();
        return;
    }

    const auto nextPosition = dynamic_cast<Ship*>(parent)->locationToPosition(nextStep) + Ship::blockSize / 2.f;
    move((nextPosition - getPosition()).normalized() * speed * deltaTime.asSeconds());
}

void Crewmate::updatePathfinding()
{
    const BlockGrid::Location gridLocation = posToGridLocation(getPosition(), static_cast<sf::Vector2u>(Ship::blockSize));
    m_steps = dynamic_cast<Ship*>(parent)->pathfind(gridLocation, targetLocation);
}

Time Crewmate::getAge() const
{
    return world->getTime() - m_birthTimestamp;
}

bool Crewmate::isAdult() const
{
    return getAge().year() >= adultYear;
}
