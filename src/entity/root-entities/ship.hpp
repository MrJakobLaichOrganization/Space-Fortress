#pragma once

#include "block.hpp"
#include "entity/attach-entities/crewmate.hpp"
#include "entity/attach-entities/machine.hpp"
#include "entity/attach-entities/work.hpp"
//DEBUGGING
#include "entity/attach-entities/chest.hpp"
#include "entity/entity.hpp"
#include "entity/root-entity.hpp"
#include "graphics/tilemap.hpp"
#include "pathfinding.hpp"
#include "resources.hpp"
#include "task.hpp"
#include "world.hpp"

class Ship : public RootEntity
{
public:
    static constexpr sf::Vector2f blockSize{64.f, 64.f};
    static constexpr sf::Vector2u dimension{32, 32};

    TileRenderer tileRenderer{Resources::get().tileSheet, dimension, blockSize};
    BlockGrid grid{dimension, &tileRenderer};
    std::vector<std::unique_ptr<Machine>> machines;
    std::vector<Task> tasks;
    std::vector<Task> takenTasks;
    std::vector<std::pair<Entity::Id, BlockGrid::Location>> tileEntities;

    template <typename T, typename... Args>
    void addMachine(std::string_view archetypeName, BlockGrid::Location location, Args&&... args)
    {
        machines.emplace_back(std::make_unique<T>(location, std::forward<Args>(args)...));
        grid.setBlockType(grid.getBlockArchetypeIdx(archetypeName), location, machines.back()->direction);
    }
    void removeMachine(BlockGrid::Location location)
    {
        auto machineIter = std::find_if(machines.begin(),
                                        machines.end(),
                                        [&location](const std::unique_ptr<Machine>& machine)
                                        { return machine->location == location; });

        if (machineIter == machines.end())
        {
            return;
        }

        // Remove the machine and its tile
        machines.erase(machineIter);
    }

    template <typename T, typename... Args>
    T& addTileEntity(std::string_view archetypeName, BlockGrid::Location location, Args&&... args)
    {
        auto& tileEntity = world->createEntity<T>(location,
                                                  &grid,
                                                  grid.getBlockArchetypeIdx(archetypeName),
                                                  std::forward<Args>(args)...);
        tileEntities.push_back(std::make_pair(tileEntity.id, location));
        return tileEntity;
    }
    void removeTileEntity(BlockGrid::Location location)
    {
        const auto iter = std::find_if(tileEntities.begin(),
                                       tileEntities.end(),
                                       [&location](const auto& entityPair) { return entityPair.second == location; });
        if (iter == tileEntities.end())
        {
            return;
        }

        world->destroyEntity(iter->first);
        tileEntities.erase(iter);
    }

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
                grid.setFloorType(floorTile, {x + 1, y + 2});
                grid.setFloorType(floorTile, {x + 1, 1});
            }
        }

        // Debug purposes
        //addMachine<Workstation>("TablePapers", {3, 3}, world, Direction::Up);
        auto& station = addTileEntity<Workstation>("TablePapers", {3, 3});
        station.bills.emplace_back(100);

        addTileEntity<Chest>("Chest", {4, 4}, Direction::Up, 100);

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
            tileRenderer.setTile(machine->location, {0, machine->tileIdx, machine->direction}, false);
        }

        RootEntity::update(delta);
    }

    std::vector<Workstation*> getWorkstations() const
    {
        std::vector<Workstation*> stations;

        for (const auto& tileEntity : tileEntities)
        {
            if (auto* tmp = dynamic_cast<Workstation*>(world->findEntity(tileEntity.first)))
            {
                stations.push_back(tmp);
            }
        }

        return stations;
    }

    sf::Vector2f locationToPosition(BlockGrid::Location location) const
    {
        return {location.x * blockSize.x, location.y * blockSize.y};
    }

    std::vector<BlockGrid::Location> pathfind(BlockGrid::Location start, BlockGrid::Location end) const
    {
        return generatePath(grid, start, end);
    }
};