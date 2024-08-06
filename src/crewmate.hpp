#pragma once

#include "entity.hpp"
#include "time.hpp"
#include <string>

class World;
class Crewmate: public Entity
{
  public:
	static constexpr std::uint64_t adultYear = 16;	// Year at which they are adult

	enum class Gender : std::uint8_t
	{
		MALE,
		FEMALE
	};

	Crewmate(const World &w, std::string_view name, Gender gender = Gender::MALE);
	Crewmate(Crewmate &&other) noexcept;

	void update(sf::Time deltaTime) override;

	bool isAdult() const;
	Time getBirthTimeStamp() const { return m_birthTimestamp; }
	Time getAge() const;
	const std::string &getName() const { return m_name; }

  private:
	std::string m_name;
	Gender m_gender;
	Time m_birthTimestamp;
	const World &m_world;
};