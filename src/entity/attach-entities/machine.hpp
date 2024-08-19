#pragma once

#include "block.hpp"
#include "entity/attach-entity.hpp"
#include "time.hpp"

#include <string>

class Machine
{
public:
    Machine(BlockGrid::Location location) : location{location}
    {
    }

    virtual ~Machine() = default;

    virtual void update(sf::Time deltaTime, class Ship& ship) = 0;

    BlockGrid::Location location;
    Direction direction{};
    std::uint32_t tileIdx{};

private:
};