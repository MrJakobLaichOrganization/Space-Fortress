#pragma once

#include "entity.hpp"
#include "units.hpp"

#include <SFML/Graphics.hpp>

#include <concepts>
#include <type_traits>

class AttachEntity : public Entity
{
public:
    class RootEntity* parent{};

    AttachEntity(class World* world, Id id) : Entity{world, id}
    {
    }

    AttachEntity(const AttachEntity&) = delete;
    AttachEntity(AttachEntity&&) = default;
};