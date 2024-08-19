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

    void attachChild(AttachEntity* child)
    {
        child->parent = this;
        children.push_back(child);
    }

    virtual void prePhysics()
    {
        if (body)
        {
            body->SetTransform(toBox2d(getPosition()), getRotation().asRadians());
        }
    }

    virtual void postPhysics()
    {
        if (body)
        {
            setPosition(toSFML(body->GetPosition()));
            setRotation(sf::radians(body->GetAngle()));
        }
    }

    void update(sf::Time deltaTime) override
    {
        for (auto& entity : children)
        {
            entity->update(deltaTime);
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();

        for (const auto& entity : children)
        {
            entity->draw(target, states);
        }
    }
};
