#pragma once

#include <SFML/Graphics/Rect.hpp>

#include <SFML/System/Vector2.hpp>

#include <array>
#include <box2d/box2d.h>

#include <cstdint>
#include <cmath>

using Index = std::int32_t;
using Distance = float;

using Position = sf::Vector2<Distance>;
using Location = sf::Vector2<Index>;

using Size = Position;
using Dimension = Location;

using Bounds = sf::Rect<Index>;
using Zone = sf::Rect<Distance>;

constexpr float meterToPixels = 64.f;

inline b2Vec2 toBox2d(Position v)
{
    return {v.x / meterToPixels, v.y / meterToPixels};
}

inline Position toSFML(b2Vec2 v)
{
    return {v.x * meterToPixels, v.y * meterToPixels};
}

enum class Direction
{
    Up,
    Right,
    Down,
    Left
};

namespace Offset
{
static constexpr Location left{-1, 0};
static constexpr Location topLeft{-1, -1};
static constexpr Location top{0, -1};
static constexpr Location topRight{1, -1};
static constexpr Location right{1, 0};
static constexpr Location bottomRight{1, 1};
static constexpr Location bottom{0, 1};
static constexpr Location bottomLeft{-1, 1};

static constexpr std::array dirs8{left, topLeft, top, topRight, right, bottomRight, bottom, bottomLeft};

} // namespace Offset

inline sf::Angle directionToAngle(Direction direction)
{
    switch (direction)
    {
        case Direction::Up:
            return sf::degrees(270);
        case Direction::Right:
            return sf::degrees(0);
        case Direction::Down:
            return sf::degrees(90);
        case Direction::Left:
            return sf::degrees(180);
    }

    assert(false);
    return {};
}

inline sf::Vector2f directionToPosition(Direction direction)
{
    switch (direction)
    {
        case Direction::Up:
            return {0, -1};
        case Direction::Right:
            return {1, 0};
        case Direction::Down:
            return {0, 1};
        case Direction::Left:
            return {-1, 0};
    }

    assert(false);
    return {};
}

inline Location directionToLocation(Direction direction)
{
    switch (direction)
    {
        case Direction::Up:
            return {0, -1};
        case Direction::Right:
            return {1, 0};
        case Direction::Down:
            return {0, 1};
        case Direction::Left:
            return {-1, 0};
    }

    assert(false);
    return {};
}

constexpr sf::Angle lerp(sf::Angle a, sf::Angle b, float t)
{
    auto diff = b.wrapUnsigned() - a.wrapUnsigned();
    constexpr auto halfTurn = sf::degrees(180);
    constexpr auto fullTurn = sf::degrees(360);
    if (diff > halfTurn)
    {
        diff -= fullTurn;
    }
    else if (diff < -halfTurn)
    {
        diff += fullTurn;
    }

    return sf::radians(std::lerp(a.asRadians(), (a + diff).asRadians(), t));
}