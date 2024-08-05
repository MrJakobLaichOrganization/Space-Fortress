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
	Crewmate(Crewmate &&other) noexcept;

	void Update(sf::Time deltaTime) override;

	bool IsAdult() const;
	Time GetBirthTimeStamp() const { return birthTimestamp; }
	Time GetAge() const;
	const std::string &GetName() const { return name; }

  private:
	std::string name;
	Gender gender;
	Time birthTimestamp;
	const World &world;
};