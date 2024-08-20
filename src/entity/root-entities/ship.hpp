#pragma once

#include "block.hpp"
#include "entity/attach-entities/crewmate.hpp"
#include "entity/attach-entities/machine.hpp"
#include "entity/entity.hpp"
#include "entity/root-entity.hpp"
#include "graphics/tilemap.hpp"
#include "resources.hpp"

class Ship : public RootEntity
{
public:
    static constexpr sf::Vector2f blockSize{64.f, 64.f};
    static constexpr sf::Vector2u dimension{32, 32};

    TileRenderer tileRenderer{Resources::get().tileSheet, dimension, blockSize};
    BlockGrid grid{dimension, &tileRenderer};
    std::vector<std::unique_ptr<Machine>> machines;

    template <typename T, typename... Args>
    void addMachine(BlockGrid::Location location, Args&&... args)
    {
        machines.emplace_back(std::make_unique<T>(location, std::forward<Args>(args)...));
    }

    Ship(class World* world, Id id) : RootEntity{world, id}
    {
        const auto floorTile = grid.getBlockArchetypeIdx("Floor");

        //grid.setBlockType(grid.getBlockArchetypeIdx("Wall_TL"), {0, 0});
        //grid.setBlockType(grid.getBlockArchetypeIdx("Wall_TR"), {6, 0});

        //grid.setBlockType(grid.getBlockArchetypeIdx("Wall_BL"), {0, 7});
        //grid.setBlockType(grid.getBlockArchetypeIdx("Wall_BR"), {6, 7});

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
        updatePhysicFixturesv2();
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
                        const b2Vec2 boxSize = toBox2d(blockSize / 2.f);
                        dynamicBox.SetAsBox(boxSize.x, boxSize.y, {x + 0.5f, y + 0.5f}, 0.0f);
                        body->CreateFixture(&fixtureDef);
                        continue;
                    }
                }

                // If only moved horizontal
                if (leftPos.y == rightPos.y)
                {
                    const b2Vec2 boxSize = toBox2d(sf::Vector2f{(rightPos.x - leftPos.x) * blockSize.x, blockSize.y} / 2.f);
                    const b2Vec2 center = toBox2d(
                        sf::Vector2f{(rightPos.x + leftPos.x) * blockSize.x / 2.f, (y + 0.5f) * blockSize.y});
                    dynamicBox.SetAsBox(boxSize.x, boxSize.y, center, 0.0f);
                }
                else
                {
                    const b2Vec2 boxSize = toBox2d(sf::Vector2f{blockSize.x, (rightPos.y - leftPos.y) * blockSize.y} / 2.f);
                    const b2Vec2 center = toBox2d(
                        sf::Vector2f{(x + 0.5f) * blockSize.x, (rightPos.y + leftPos.y) * blockSize.y / 2.f});
                    dynamicBox.SetAsBox(boxSize.x, boxSize.y, center, 0.0f);
                }
                body->CreateFixture(&fixtureDef);
            }
        }
    }

    void updatePhysicFixturesv2()
    {
        std::vector<b2Vec2> hull;

        const auto isBlocked = [&](sf::Vector2i pos)
        {
            if (pos.x < 0 || pos.y < 0 || pos.x >= grid.getDimension().x || pos.y >= grid.getDimension().y)
            {
                return false;
            }

            return grid.getBlockArchetype(sf::Vector2u{pos}).solid;
        };

        const auto canMove = [&](sf::Vector2i pos, int d)
        {
            if (d == 0)
            {
                return !isBlocked({pos.x, pos.y - 1}) && isBlocked({pos.x - 1, pos.y - 1});
            }
            else if (d == 1)
            {
                return !isBlocked({pos.x, pos.y}) && isBlocked({pos.x, pos.y - 1});
            }
            else if (d == 2)
            {
                return isBlocked({pos.x, pos.y}) && !isBlocked({pos.x - 1, pos.y});
            }
            else if (d == 3)
            {
                return !isBlocked({pos.x - 1, pos.y - 1}) && isBlocked({pos.x - 1, pos.y});
            }

            return false;
        };

        const auto startBlock = [&]()
        {
            for (int y = 0; y < grid.getDimension().y; ++y)
            {
                for (int x = 0; x < grid.getDimension().x; ++x)
                {
                    if (isBlocked({x, y}))
                    {
                        return sf::Vector2i{x, y};
                    }
                }
            }

            return sf::Vector2i{};
        }();

        auto currentBlock = startBlock;

        std::array<sf::Vector2i, 4> dirs{{
            {0, -1},
            {1, 0},
            {0, 1},
            {-1, 0},
        }};

        int dir = 1;

        do
        {
            for (int x = 0; x < 4; x++)
            {
                if (canMove(currentBlock, dir))
                {
                    break;
                }

                dir = (dir + 1) % 4;
            }

            hull.push_back(toBox2d(locationToPosition({currentBlock})));
            currentBlock += dirs[dir];
        } while (currentBlock != startBlock);

        while (body->GetFixtureList())
        {
        body->DestroyFixture(body->GetFixtureList());

        }

        b2ChainShape chain;
        chain.CreateLoop(hull.data(), hull.size() - 1);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &chain;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.restitution = 0.5f;
        body->CreateFixture(&fixtureDef);

        const auto blockSurface = toBox2d(blockSize).x * toBox2d(blockSize).y;
        const auto blockMass = blockSurface * 10.f;

        sf::Vector2f massCenter;
        int massCount{};
        float mass{};
        for (int y = 0; y < grid.getDimension().y; ++y)
        {
            for (int x = 0; x < grid.getDimension().x; ++x)
            {
                if (isBlocked({x, y}))
                {
                    massCount++;
                    mass += blockMass;
                    massCenter += sf::Vector2f(x + 0.5f, y + 0.5f);
                }
            }
        }

        b2MassData massData;
        massData.mass = mass;
        massData.center = toBox2d(massCenter / static_cast<float>(massCount) * blockSize.x);
        //massData.I = massData.mass * (0.5f * blockSurface + b2Dot(massData.center, massData.center));
        massData.I = mass * mass;
        body->SetMassData(&massData);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        const auto originalStates = states;

        states.transform *= getTransform();
        target.draw(tileRenderer, states);

        RootEntity::draw(target, originalStates);
    }

    void update(sf::Time delta) override
    {
        for (auto& machine : machines)
        {
            machine->update(delta, *this);
            tileRenderer.setTile(machine->location, {machine->tileIdx, machine->direction});
        }

        RootEntity::update(delta);
    }

    sf::Vector2f locationToPosition(BlockGrid::Location location)
    {
        return {location.x * blockSize.x, location.y * blockSize.y};
    }
};