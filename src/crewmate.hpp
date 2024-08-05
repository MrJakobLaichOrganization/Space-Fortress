#pragma once

#include "entity.hpp"
#include "time.hpp"
#include <string>

class World;
class Crewmate: public Entity
{
  public:
	static constexpr std::uint64_t AdultYear = 16;	// Year at which they are adult

	enum class Gender : std::uint8_t
	{
		MALE,
		FEMALE
	};

	Crewmate(const World &w, std::string_view name, Gender gender = Gender::MALE);

	void Update(sf::Time deltaTime) override;

	bool Adult() const { return adult; }
	Time BirthTimeStamp() const { return birthTimestamp; }
	Time Age() const;
	const std::string &Name() const { return name; }

  private:
	std::string name;
	Gender gender;
	Time birthTimestamp;
	const World &world;
	bool adult = false;		// Cannot work if not adult
};