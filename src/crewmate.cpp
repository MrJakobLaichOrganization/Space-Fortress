#include "crewmate.hpp"
#include "world.hpp"

#include <iostream>

Crewmate::Crewmate(const World &world_, std::string_view name_, Gender gender_)
	: name(name_), gender(gender_), world(world_)
{
	birthTimestamp = world.GetTime();
}
Crewmate::Crewmate(Crewmate &&other) noexcept
	: name(std::exchange(other.name, "")), gender(other.gender), world(other.world), 
	birthTimestamp(std::exchange(other.birthTimestamp, Time()))
{
}

void Crewmate::Update(sf::Time deltaTime)
{
}

Time Crewmate::GetAge() const { return world.GetTime() - birthTimestamp; }
bool Crewmate::IsAdult() const { return GetAge().Year() >= AdultYear; }