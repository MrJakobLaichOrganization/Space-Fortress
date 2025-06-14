#pragma once

#include "attach-entity.hpp"
#include "entity.hpp"
#include "units.hpp"

#include <SFML/Graphics.hpp>

#include <concepts>
#include <type_traits>

class RootEntity : public Entity
{
public:
    b2Body* body{};

    std::vector<class AttachEntity*> children;

    RootEntity(class World* world, Id id) : Entity{world, id}
    {
    }

    RootEntity(const RootEntity&) = delete;
    RootEntity(RootEntity&&) = default;

    ~RootEntity() override;

    void attachChild(AttachEntity* child);

    virtual void prePhysics();

    virtual void postPhysics();

    void update(sf::Time deltaTime) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
