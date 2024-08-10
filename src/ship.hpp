#pragma once

#include "block.hpp"
#include "crewmate.hpp"
#include "entity.hpp"
#include "graphics/tilemap.hpp"
#include "resources.hpp"
#include "root-entity.hpp"

class Ship : public RootEntity
{
public:
    Tilemap tilemap{Resources::get().tilesTexture, {32, 32}, {32, 32}};
    BlockGrid grid{{32, 32}, &tilemap};

    Ship(Id id) : RootEntity{id}
    {
        const auto floorTile = grid.getBlockArchetypeIdx("Floor");

        grid.setBlockType(grid.getBlockArchetypeIdx("Wall_TL"), {0, 0});
        grid.setBlockType(grid.getBlockArchetypeIdx("Wall_TR"), {6, 0});

        grid.setBlockType(grid.getBlockArchetypeIdx("Wall_BL"), {0, 7});
        grid.setBlockType(grid.getBlockArchetypeIdx("Wall_BR"), {6, 7});

        for (std::uint32_t x = 0; x < 5; x++)
        {
            grid.setBlockType(grid.getBlockArchetypeIdx("Wall_MU"), {x + 1, 0});
            grid.setBlockType(grid.getBlockArchetypeIdx("Wall_MD"), {x + 1, 7});
            grid.setBlockType(grid.getBlockArchetypeIdx("Wall_ML"), {0, x + 1});
            grid.setBlockType(grid.getBlockArchetypeIdx("Wall_MR"), {6, x + 1});
            for (std::uint32_t y = 0; y < 5; y++)
            {
                grid.setBlockType(floorTile, {x + 1, y + 2});
                grid.setBlockType(floorTile, {x + 1, 1});
            }
        }

        grid.setBlockType(grid.getBlockArchetypeIdx("Wall_ML"), {0, 6});
        grid.setBlockType(grid.getBlockArchetypeIdx("Wall_MR"), {6, 6});

        grid.setBlockType(grid.getBlockArchetypeIdx("DoorClosed"), {2, 0});
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        const auto originalStates = states;

        states.transform *= getTransform();
        target.draw(tilemap, states);

        RootEntity::draw(target, originalStates);
    }

    void update(sf::Time delta) override
    {

        RootEntity::update(delta);
    }
};