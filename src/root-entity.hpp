#pragma once

#include "units.hpp"

#include <SFML/Graphics.hpp>

#include <concepts>
#include <type_traits>

#include "entity.hpp"
#include "attach-entity.hpp"

class RootEntity : public Entity
{
public:
    b2Body* body{};

    std::vector<class AttachEntity*> children;

    RootEntity(Entity::Id id) : Entity{id}
    {
    }

    RootEntity(const RootEntity&) = delete;
    RootEntity(RootEntity&&) = default;

    void attachChild(AttachEntity* child)
    {
        child->parent = this;
        children.push_back(child);
    }

    virtual void prePhysics()
    {
        if (body)
        {
            body->SetTransform(toBox2d(getPosition() / meterToPixels), getRotation().asRadians());
        }
    }

    virtual void postPhysics()
    {
        if (body)
        {
            setPosition(toSFML(body->GetPosition()) * meterToPixels);
            setRotation(sf::radians(body->GetAngle()));
        }
    }

    virtual void update(sf::Time deltaTime)
    {
        for (auto& entity : children)
        {
            entity->update(deltaTime);
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();

        for (auto& entity : children)
        {
            entity->draw(target, states);
        }
    }
};
