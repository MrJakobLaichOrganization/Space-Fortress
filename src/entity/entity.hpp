#pragma once

#include "units.hpp"

#include <SFML/Graphics.hpp>

#include <concepts>
#include <type_traits>

class Entity : public sf::Transformable, public sf::Drawable
{
public:
    using Id = std::int64_t;

    Id id;
    class World* world;

    Entity(class World* world, Id id) : id{id}, world{world}
    {
    }

    Entity(const Entity&) = delete;
    Entity(Entity&&) = default;

    virtual void update(sf::Time deltaTime)
    {
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
    }
};

template <typename T>
concept EntityDerivation = std::is_base_of_v<Entity, T>;