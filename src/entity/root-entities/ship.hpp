#pragma once

#include "block.hpp"
#include "entity/attach-entities/crewmate.hpp"
#include "entity/entity.hpp"
#include "graphics/tilemap.hpp"
#include "resources.hpp"

class Ship : public RootEntity
{
public:
    Tilemap tilemap{Resources::get().tilesTexture, {32, 32}, {32, 32}};
    BlockGrid grid{{32, 32}, &tilemap};

    Ship(class World* world, Id id) : RootEntity{world, id}
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


        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        body = world->getPhysicsWorld().CreateBody(&bodyDef);

        updatePhysicFixtures();
    }

    void updatePhysicFixtures()
    {
        Grid<std::uint8_t> solidTiles(grid.getDimension());

        b2PolygonShape dynamicBox;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.restitution = 0.5f;

        // Fill the solid tiles
        for (std::uint32_t y = 0; y < grid.getDimension().y; ++y)
        {
            for (std::uint32_t x = 0; x < grid.getDimension().x; ++x)
            {
                solidTiles.set({x, y}, grid.getBlockArchetype(sf::Vector2u(x, y)).solid);
            }
        }

        // Create the box strips
        for (std::uint32_t y = 0; y < grid.getDimension().y; ++y)
        {
            for (std::uint32_t x = 0; x < grid.getDimension().x; ++x)
            {
                if (!solidTiles.get({x, y}))
                {
                    continue;
                }
                solidTiles.set({x, y}, 0);

                sf::Vector2u leftPos;
                leftPos.x = x;
                leftPos.y = y;
                sf::Vector2u rightPos;
                rightPos.x = x + 1;
                rightPos.y = y;

                // Go left
                while (leftPos.x != 0 && leftPos.x - 1 > 0)
                {
                    const auto idx = solidTiles.locationToIndex(leftPos);
                    if (idx >= solidTiles.getCount() || !solidTiles.get(idx))
                    {
                        break;
                    }

                    leftPos.x--;
                    solidTiles.set(leftPos, 0);
                }

                // Go right
                do
                {
                    const auto idx = solidTiles.locationToIndex(rightPos);
                    if (idx >= solidTiles.getCount() || !solidTiles.get(idx))
                    {
                        break;
                    }

                    solidTiles.set(rightPos, 0);
                    rightPos.x++;
                } while (rightPos.x <= grid.getDimension().x);

                // If no movement done, move vertically
                if (leftPos == sf::Vector2u{rightPos.x - 1, rightPos.y})
                {
                    rightPos.x -= 1;
                    rightPos.y++;
                    // Go up
                    while (leftPos.y > 0)
                    {
                        const auto idx = solidTiles.locationToIndex(leftPos);
                        if (idx >= solidTiles.getCount() || !solidTiles.get(idx))
                        {
                            break;
                        }
                        solidTiles.set(leftPos, 0);
                        leftPos.y--;
                    }

                    // Go down
                    do
                    {
                        const auto idx = solidTiles.locationToIndex(rightPos);
                        if (idx >= solidTiles.getCount() || !solidTiles.get(idx))
                        {
                            break;
                        }
                        solidTiles.set(rightPos, 0);
                        rightPos.y++;
                    } while (rightPos.x < grid.getDimension().y);

                    // If no bigger box still, make one small
                    if (leftPos == sf::Vector2u{rightPos.x, rightPos.y - 1})
                    {
                        dynamicBox.SetAsBox(1.f / 2.f, 1.f / 2.f, {x + 0.5f, y + 0.5f}, 0.0f);
                        body->CreateFixture(&fixtureDef);
                        continue;
                    }
                }

                b2Vec2 center{};
                // If only moved horizontal
                if (leftPos.y == rightPos.y)
                {
                    center.x = (rightPos.x + leftPos.x) / 2.f;
                    center.y = y + 0.5f;
                    dynamicBox.SetAsBox((rightPos.x - leftPos.x) / 2.f, 1.f / 2.f, center, 0.0f);
                }
                else
                {
                    center.y = (rightPos.y + leftPos.y) / 2.f;
                    center.x = x + 0.5f;
                    dynamicBox.SetAsBox(1.f / 2.f, (rightPos.y - leftPos.y) / 2.f, center, 0.0f);
                }
                body->CreateFixture(&fixtureDef);
            }
        }
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