#pragma once

#include "block.hpp"
#include "entity/attach-entities/work.hpp"
#include "entity/attach-entity.hpp"
#include "task/task.hpp"
#include "time.hpp"

#include <SFML/System/Vector2.hpp>

#include <queue>
#include <string>

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
    ~Crewmate() override;

    void update(sf::Time deltaTime) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();

        sf::CircleShape circle(localBounds.size.x / 2.f);
        circle.setFillColor(sf::Color::Yellow);
        circle.setOrigin(-localBounds.position);
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

    Location targetLocation;
    float speed = 50.f;

    static Location posToGridLocation(Position pos, Size tileSize)
    {
        return Location{static_cast<Index>(pos.x / tileSize.x), static_cast<Index>(pos.y / tileSize.y)};
    }

private:
    std::vector<Location> m_steps;

    std::string m_name;
    Gender m_gender;
    Time m_birthTimestamp;
    std::unique_ptr<Task> m_currentTask;
    Entity::Id m_currentWorkstation{Entity::invalidID};

    void step(sf::Time deltaTime);
    void work(sf::Time deltaTime);
    void updatePathfinding();
};