#include "crewmate.hpp"
#include "world.hpp"

#include <iostream>

Crewmate::Crewmate(const World &w, std::string_view name_, Gender gend)
	: name(name_), gender(gend), world(w)
{
	birthTimestamp = world.GetTime();
}
void Crewmate::Update(sf::Time deltaTime)
{
	// Quick check if adult or not
	if (!adult && (world.GetTime().Year() - birthTimestamp.Year()) >= AdultYear)
	{
		adult = true;
	}
}

Time Crewmate::Age() const { return world.GetTime() - birthTimestamp; }