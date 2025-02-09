#pragma once

#include "block.hpp"
#include "entity/attach-entities/crewmate.hpp"
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
    enum class Layer
    {
        Floor,
        Main,
        Count
    };

    static constexpr sf::Vector2f blockSize{64.f, 64.f};
    static constexpr sf::Vector2u dimension{32, 32};

    std::array<TileRenderer, static_cast<int>(Layer::Count)> tileRenderers{
        {{Resources::get().tileSheet, dimension, blockSize}, {Resources::get().tileSheet, dimension, blockSize}}};

    BlockGrid grid{dimension, &tileRenderers[static_cast<int>(Layer::Floor)], &tileRenderers[static_cast<int>(Layer::Main)]};

    std::vector<Task> tasks;
    std::vector<Task> takenTasks;

    template <typename T, typename... Args>
    T& addTileEntity(std::string_view archetypeName, BlockGrid::Location location, Direction direction, Args&&... args)
    {
        auto& tileEntity = world->createEntity<T>(location,
                                                  direction,
                                                  &grid,
                                                  grid.getBlockArchetypeIdx(archetypeName),
                                                  std::forward<Args>(args)...);
        attachChild(&tileEntity);
        return tileEntity;
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
        auto& station = addTileEntity<Workstation>("TablePapers", {3, 3}, Direction::Up);
        station.bills.emplace_back(100);

        addTileEntity<Chest>("Chest", {4, 4}, Direction::Up, 100);

        grid.setBlockType(grid.getBlockArchetypeIdx("Wall_ML"), {0, 6});
        grid.setBlockType(grid.getBlockArchetypeIdx("Wall_MR"), {6, 6});

        grid.setBlockType(grid.getBlockArchetypeIdx("DoorClosed"), {2, 0});

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.userData = {this};
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
                const auto& blockData = grid.getBlockData(sf::Vector2u(x, y));
                solidTiles.set({x, y}, blockData.blockAchetypeIdx || blockData.floorAchetypeIdx);
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

                uint32_t left = x;
                uint32_t right = x;
                uint32_t top = y;
                uint32_t bottom = y;

                // Go left
                while (solidTiles.isValid({left - 1, y}))
                {
                    if (!solidTiles.get({left - 1, y}))
                    {
                        break;
                    }

                    left--;
                    solidTiles.set({left, y}, 0);
                }

                // Go right
                while (solidTiles.isValid({right + 1, y}))
                {
                    if (!solidTiles.get({right + 1, y}))
                    {
                        break;
                    }

                    right++;
                    solidTiles.set({right, y}, 0);
                }

                // Go up
                while (top > 0)
                {
                    bool valid = true;
                    for (std::uint32_t testX = left; testX <= right; testX++)
                    {
                        if (!solidTiles.get({testX, top - 1u}))
                        {
                            valid = false;
                            break;
                        }
                    }

                    if (valid)
                    {
                        top--;
                        for (std::uint32_t testX = left; testX <= right; testX++)
                        {
                            solidTiles.set({testX, top}, 0);
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                // Go down
                while (bottom + 1 < solidTiles.getDimension().y)
                {
                    bool valid = true;
                    for (std::uint32_t testX = left; testX <= right; testX++)
                    {
                        if (!solidTiles.get({testX, bottom + 1}))
                        {
                            valid = false;
                            break;
                        }
                    }

                    if (valid)
                    {
                        bottom++;
                        for (std::uint32_t testX = left; testX <= right; testX++)
                        {
                            solidTiles.set({testX, bottom}, 0);
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                right++;
                bottom++;

                const float boxSize = toBox2d(blockSize).x;
                dynamicBox.SetAsBox((right - left) * boxSize / 2.f,
                                    (bottom - top) * boxSize / 2.f,
                                    b2Vec2{(left + (right - left) / 2.f) * boxSize, (top + (bottom - top) / 2.f) * boxSize},
                                    0.0f);
                body->CreateFixture(&fixtureDef);
            }
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        const auto originalStates = states;

        states.transform *= getTransform();

        for (int x = 0; x < static_cast<int>(Layer::Count); x++)
        {
            target.draw(tileRenderers[x], states);
        }

        RootEntity::draw(target, originalStates);
    }

    void update(sf::Time delta) override
    {
        RootEntity::update(delta);
    }

    std::vector<Workstation*> getWorkstations() const
    {
        std::vector<Workstation*> stations;

        for (const auto& child : children)
        {
            if (auto* station = dynamic_cast<Workstation*>(child))
            {
                stations.push_back(station);
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