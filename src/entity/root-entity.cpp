#include "root-entity.hpp"

#include "entity/entity.hpp"

#include <world.hpp>

RootEntity::~RootEntity()
{
    while (!children.empty())
    {
        world->destroyEntity(children.back()->id);
    }
    assert(children.empty());
}

void RootEntity::attachChild(AttachEntity* child)
{
    child->parent = this;
    children.push_back(child);
}

void RootEntity::prePhysics()
{
    if (body)
    {
        body->SetTransform(toBox2d(getPosition()), getRotation().asRadians());
    }
}

void RootEntity::postPhysics()
{
    if (body)
    {
        setPosition(toSFML(body->GetPosition()));
        setRotation(sf::radians(body->GetAngle()));
    }
}

void RootEntity::update(sf::Time deltaTime)
{
    for (auto& entity : children)
    {
        entity->update(deltaTime);
    }
}

void RootEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (const auto& entity : children)
    {
        entity->draw(target, states);
    }
}