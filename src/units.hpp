#pragma once

#include <SFML/System/Vector2.hpp>

#include <box2d/box2d.h>

constexpr float meterToPixels = 64.f;

inline b2Vec2 toBox2d(sf::Vector2f v)
{
    return {v.x / meterToPixels, v.y / meterToPixels};
}

inline sf::Vector2f toSFML(b2Vec2 v)
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
            return sf::Vector2f{0, -1};
        case Direction::Right:
            return sf::Vector2f{1, 0};
        case Direction::Down:
            return sf::Vector2f{0, 1};
        case Direction::Left:
            return sf::Vector2f{-1, 0};
    }

    assert(false);
    return {};
}

inline sf::Vector2i directionToLocation(Direction direction)
{
    switch (direction)
    {
        case Direction::Up:
            return sf::Vector2i{0, -1};
        case Direction::Right:
            return sf::Vector2i{1, 0};
        case Direction::Down:
            return sf::Vector2i{0, 1};
        case Direction::Left:
            return sf::Vector2i{-1, 0};
    }

    assert(false);
    return {};
}