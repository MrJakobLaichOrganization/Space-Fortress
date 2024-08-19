#pragma once

#include "entity/attach-entities/machine.hpp"
#include "entity/root-entities/ship.hpp"
#include "time.hpp"

#include <string>

class Thruster : public Machine
{
public:
    Thruster(BlockGrid::Location location) : Machine{location}
    {
        tileIdx = 80;
        direction = Direction::Down;
    }

    void update(sf::Time /* deltaTime */, class Ship& ship) override
    {
        const auto magnitude = 1000.f;
        const auto force = -sf::Vector2f(magnitude, directionToAngle(direction) + ship.getRotation());
        ship.body->ApplyForce(toBox2d(force), toBox2d(ship.getTransform() * ship.locationToPosition(location)), true);
    }
};