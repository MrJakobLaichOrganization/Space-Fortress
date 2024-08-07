#pragma once

#include <SFML/System/Vector2.hpp>

#include <box2d/box2d.h>

constexpr float meterToPixels = 64.f;

inline b2Vec2 toBox2d(sf::Vector2f v)
{
    return {v.x, v.y};
}

inline sf::Vector2f toSFML(b2Vec2 v)
{
    return {v.x, v.y};
}