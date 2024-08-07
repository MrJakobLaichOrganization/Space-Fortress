#include "crewmate.hpp"
#include "world.hpp"

Crewmate::Crewmate(const World &world, std::string_view name, Gender gender)
	: m_name(name), m_gender(gender), m_world(world)
{
	m_birthTimestamp = m_world.getTime();
}
Crewmate::Crewmate(Crewmate &&other) noexcept
	: m_name(std::exchange(other.m_name, "")), m_gender(other.m_gender), m_world(other.m_world), 
	m_birthTimestamp(std::exchange(other.m_birthTimestamp, Time()))
{
}

void Crewmate::update(sf::Time deltaTime)
{
}

Time Crewmate::getAge() const { return m_world.getTime() - m_birthTimestamp; }
bool Crewmate::isAdult() const { return getAge().year() >= adultYear; }
