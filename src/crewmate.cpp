#include "crewmate.hpp"

#include "ship.hpp"
#include "world.hpp"

Crewmate::Crewmate(Id id, const World& world, std::string_view name, Gender gender) :
    AttachEntity{id},
    m_name(name),
    m_gender(gender),
    m_world(world)
{
    m_birthTimestamp = m_world.getTime();
}

void Crewmate::update(sf::Time deltaTime)
{
    if (m_position != m_targetPos && !m_steps.size())
    {
    }
}

Time Crewmate::getAge() const
{
    return m_world.getTime() - m_birthTimestamp;
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
        m_steps = m_world.generatePath(ship->grid,
                                       static_cast<sf::Vector2i>(m_position),
                                       static_cast<sf::Vector2i>(m_targetPos));
    }
}
