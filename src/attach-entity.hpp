#pragma once

#include "units.hpp"

#include <SFML/Graphics.hpp>

#include <concepts>
#include <type_traits>

#include "entity.hpp"

class AttachEntity : public Entity
{
public:

    class RootEntity* parent{};

    AttachEntity(Id id) : Entity{id}
    {
    }

    AttachEntity(const AttachEntity&) = delete;
    AttachEntity(AttachEntity&&) = default;

    virtual void update(sf::Time deltaTime)
    {
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
    }
};