#pragma once

#include "block.hpp"
#include "entity/attach-entities/workstation.hpp"
#include "entity/attach-entity.hpp"
#include "item/inventory.hpp"
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

        /*
        // Debug render
        sf::CircleShape circle(localBounds.size.x / 2.f);
        circle.setFillColor(sf::Color::Yellow);
        circle.setOrigin(-localBounds.position);
        target.draw(circle, states);
        */

        sf::CircleShape circle(localBounds.size.x / 2.f);
        circle.setFillColor(sf::Color{64, 154, 201, 255});
        circle.setOrigin(-localBounds.position);
        circle.setScale({0.5f, 1.f});
        target.draw(circle, states);

        circle.setFillColor(sf::Color{214, 202, 146, 255});
        circle.setScale({0.4f, 0.4f});
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

    float speed = 50.f;
    void step(Position direction, sf::Time deltaTime)
    {
        move(direction.normalized() * std::min(speed * deltaTime.asSeconds(), direction.length()));

        if (direction.length() > 0)
        {
            const auto targetAngle = direction.angle();
            setRotation(
                sf::radians(std::lerp(getRotation().asRadians(), targetAngle.asRadians(), 5.f * deltaTime.asSeconds())));
        }
    }

    static Location posToGridLocation(Position pos, Size tileSize)
    {
        return Location{static_cast<Index>(pos.x / tileSize.x), static_cast<Index>(pos.y / tileSize.y)};
    }

    Task* currentTask{};
    ActPtr currentAct;

    Inventory inventory{9999999};

private:
    std::string m_name;
    Gender m_gender;
    Time m_birthTimestamp;
};