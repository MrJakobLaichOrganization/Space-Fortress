#include "crewmate.hpp"

#include "world.hpp"

Crewmate::Crewmate(World* world, Id id, std::string_view name, Gender gender) :
    AttachEntity{world, id},
    m_name(name),
    m_gender(gender)
{
    m_birthTimestamp = world->getTime();
}

void Crewmate::update(sf::Time deltaTime)
{
}

Time Crewmate::getAge() const
{
    return world->getTime() - m_birthTimestamp;
}

bool Crewmate::isAdult() const
{
    return getAge().year() >= adultYear;
}
void Crewmate::setMoveTarget(sf::Vector2u targetPos)
{
    m_targetPos = targetPos;
    Ship* ship;
    if ((ship = dynamic_cast<Ship*>(parent)))
    {
        m_steps = generatePath(ship->grid, static_cast<sf::Vector2i>(m_position), static_cast<sf::Vector2i>(m_targetPos));
    }
}
