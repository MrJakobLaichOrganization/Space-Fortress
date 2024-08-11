#pragma once

#include "entity/attach-entity.hpp"
#include <SFML/System/Vector2.hpp>

#include "time.hpp"

#include <string>
#include <queue>

class World;
class Crewmate : public AttachEntity
{
public:
    static constexpr std::uint64_t adultYear = 16; // Year at which they are adult

    enum class Gender : std::uint8_t
    {
        MALE,
        FEMALE
    };

    Crewmate(class World* world, Id id, std::string_view name, Gender gender = Gender::MALE);
    Crewmate(Crewmate&& other) noexcept = default;

    void update(sf::Time deltaTime) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();

        sf::CircleShape circle(16.f);
        circle.setFillColor(sf::Color::Yellow);
        target.draw(circle, states);
    }

    bool isAdult() const;

    Time getBirthTimeStamp() const
    {
        return m_birthTimestamp;
    }

    Time getAge() const;

    const std::string& getName() const
    {
        return m_name;
    }

    void setMoveTarget(sf::Vector2u targetPos);

private:
    sf::Vector2u m_position{};
    sf::Vector2u m_targetPos{};
    std::queue<sf::Vector2i> m_steps{}; // offset compared to the next block

    std::string m_name;
    Gender m_gender;
    Time m_birthTimestamp;
};