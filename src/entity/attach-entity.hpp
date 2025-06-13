#pragma once

#include "box2d/b2_fixture.h"
#include "entity.hpp"
#include "units.hpp"

#include <SFML/Graphics.hpp>

#include <concepts>
#include <type_traits>

class AttachEntity : public Entity
{
public:
    class RootEntity* parent{};
    sf::FloatRect localBounds;

    AttachEntity(class World* world, Id id) : Entity{world, id}
    {
    }

    AttachEntity(const AttachEntity&) = delete;
    AttachEntity(AttachEntity&&) = default;

    ~AttachEntity();
};