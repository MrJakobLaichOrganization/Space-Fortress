#include "crewmate.hpp"

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
}

Time Crewmate::getAge() const
{
    return m_world.getTime() - m_birthTimestamp;
}

bool Crewmate::isAdult() const
{
    return getAge().year() >= adultYear;
}
