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

    AttachEntity(Id id) : Entity{id}
    {
    }

    AttachEntity(const AttachEntity&) = delete;
    AttachEntity(AttachEntity&&) = default;
};