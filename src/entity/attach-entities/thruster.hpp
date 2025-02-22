#pragma once

#include "entity/root-entities/ship.hpp"
#include "time.hpp"

#include <string>

class Thruster : public TileEntity
{
public:
    Thruster(World* world, Entity::Id id, Location location, Direction direction, BlockGrid* grid, BlockArchetypeIndex idx) :
        TileEntity(world, id, location, direction, grid, idx)
    {
        //tileIdx = 80;
    }

    void update(sf::Time /* deltaTime */) override
    {
        auto& ship = *static_cast<Ship*>(parent);
        const auto magnitude = 1000.f;
        const auto force = -Position(magnitude, directionToAngle(m_direction) + ship.getRotation());
        ship.body->ApplyForce(toBox2d(force), toBox2d(ship.getTransform() * ship.locationToPosition(m_location)), true);
    }
};