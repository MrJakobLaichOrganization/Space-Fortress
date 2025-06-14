#pragma once

#include "block.hpp"
#include "entity/attach-entities/crewmate.hpp"
#include "entity/attach-entities/workstation.hpp"

#include <algorithm>
#include <random>
//DEBUGGING
#include "entity/attach-entities/chest.hpp"
#include "entity/attach-entity.hpp"
#include "entity/entity.hpp"
#include "entity/root-entity.hpp"
#include "graphics/tilemap.hpp"
#include "pathfinding.hpp"
#include "resources.hpp"
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

    static constexpr Size blockSize{64.f, 64.f};
    static constexpr Bounds bounds{{-32, -32}, {64, 64}};

    std::array<TileRenderer, static_cast<int>(Layer::Count)> tileRenderers{
        {{Resources::get().tileSheet, bounds, blockSize}, {Resources::get().tileSheet, bounds, blockSize}}};

    BlockGrid grid{bounds, &tileRenderers[static_cast<int>(Layer::Floor)], &tileRenderers[static_cast<int>(Layer::Main)]};

    std::vector<std::unique_ptr<Task>> tasks;

    template <typename T, typename... Args>
    T& addTileEntity(std::string_view archetypeName, Location location, Direction direction, Args&&... args)
    {
        auto& tileEntity = world->createEntity<T>(location,
                                                  direction,
                                                  &grid,
                                                  grid.getBlockArchetypeIdx(archetypeName),
                                                  std::forward<Args>(args)...);
        attachChild(&tileEntity);
        return tileEntity;
    }

    template <typename T, typename... Args>
    T& addTask(Args&&... args)
    {
        return static_cast<T&>(*tasks.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)));
    }

    void removeTask(Task& task)
    {
        if (task.worker)
        {
            task.worker->currentTask = nullptr;
            task.worker = nullptr;
        }
        std::erase_if(tasks, [&](const auto& ptr) { return ptr.get() == &task; });
    }

    Task* takeTask(Crewmate& taskee)
    {
        const auto it = std::ranges::find_if(tasks, [](const auto& t) { return !t->worker; });
        if (it == tasks.end())
        {
            return nullptr;
        }

        Task* task = it->get();
        assert(!task->worker);

        task->worker = &taskee;
        taskee.currentTask = task;

        return task;
    }

    Ship(class World* world, Id id) : RootEntity{world, id}
    {
        const auto floorTile = grid.getBlockArchetypeIdx("Floor");

        grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {0, 0});
        grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {6, 0});

        grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {0, 7});
        grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {6, 7});

        for (Index x = 0; x < 5; x++)
        {
            grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {x + 1, 0});
            grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {x + 1, 7});
            grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {0, x + 1});
            grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {6, x + 1});
            for (Index y = 0; y < 5; y++)
            {
                grid.setFloorType(floorTile, {x + 1, y + 2});
                grid.setFloorType(floorTile, {x + 1, 1});
            }
        }

        {
            for (Index x = 0; x < 4; x++)
            {
                grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {x + 1, 2});
            }
            for (Index x = 0; x < 4; x++)
            {
                grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {0 - x, 2});
            }
        }

        // Debug purposes
        auto& station = addTileEntity<Workstation>("TablePapers", {1, 6}, Direction::Up, Direction::Up);
        station.addBill({100});
        station.addBill({100});
        station.addBill({100});

        addTileEntity<Chest>("Chest", {4, 3}, Direction::Up, 100);

        grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {0, 6});
        grid.setBlockType(grid.getBlockArchetypeIdx("Wall"), {6, 6});

        grid.setBlockType(grid.getBlockArchetypeIdx("DoorClosed"), {2, 0});

        const auto targetPosition = locationToPosition({1, 4}) + Ship::blockSize / 2.f;
        addTask<MoveTask>(targetPosition);

        std::random_device r;
        std::default_random_engine e1(r());

        for (int x = 0; x < 5; x++)
        {
            std::uniform_int_distribution<int> xDist(1, 5);
            std::uniform_int_distribution<int> yDist(1, 6);

            const auto rndPos = locationToPosition({xDist(e1), yDist(e1)}) + Ship::blockSize / 2.f;
            addTask<MoveTask>(rndPos);
        }

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.userData = {this};
        body = world->getPhysicsWorld().CreateBody(&bodyDef);

        updatePhysicFixtures();
    }

    ~Ship() override
    {
        while (!children.empty())
        {
            world->destroyEntity(children.back()->id);
        }
    }


    void updatePhysicFixtures()
    {
        OffsetGrid<std::uint8_t> solidTiles(grid.getBounds());

        b2PolygonShape dynamicBox;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.restitution = 0.5f;

        const auto min = grid.getMin();
        const auto max = grid.getMax();

        // Fill the solid tiles
        for (Index y = min.y; y < max.y; ++y)
        {
            for (Index x = min.x; x < max.x; ++x)
            {
                const auto& blockData = grid.getBlockData({x, y});
                solidTiles.set({x, y}, blockData.blockAchetypeIdx || blockData.floorAchetypeIdx);
            }
        }

        // Create the box strips
        for (Index y = min.y; y < max.y; ++y)
        {
            for (Index x = min.x; x < max.x; ++x)
            {
                if (!solidTiles.get({x, y}))
                {
                    continue;
                }
                solidTiles.set({x, y}, 0);

                Index left = x;
                Index right = x;
                Index top = y;
                Index bottom = y;

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
                while (top > min.y)
                {
                    bool valid = true;
                    for (Index testX = left; testX <= right; testX++)
                    {
                        if (!solidTiles.get({testX, top - 1}))
                        {
                            valid = false;
                            break;
                        }
                    }

                    if (valid)
                    {
                        top--;
                        for (Index testX = left; testX <= right; testX++)
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
                while (bottom + 1 < max.y)
                {
                    bool valid = true;
                    for (Index testX = left; testX <= right; testX++)
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
                        for (Index testX = left; testX <= right; testX++)
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

                const Distance boxSize = toBox2d(blockSize).x;
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

        for (auto workstation : getWorkstations())
        {
            const auto progress = workstation->getProgress();
            if (progress < 0.f)
            {
                continue;
            }

            const auto pos = locationToPosition(workstation->getLocation()) + blockSize / 2.f;

            sf::RectangleShape background({blockSize.x * 0.8f, blockSize.y * 0.2f});
            background.setOrigin({0.f, background.getSize().y});
            background.setFillColor(sf::Color::Black);
            background.setPosition(pos + sf::Vector2f{-background.getSize().x / 2.f, -blockSize.y * 0.4f});

            auto foreground{background};

            background.setOutlineColor(sf::Color::Black);
            background.setOutlineThickness(5.f);
            target.draw(background, states);

            foreground.setScale({1.f - progress, 1.f});
            foreground.setFillColor(sf::Color::Blue);
            target.draw(foreground, states);

            sf::Text num(Resources::get().imbPlexMono, std::to_string(workstation->getBillCount()), 30);
            num.setFillColor(sf::Color::White);

            const auto numBounds = num.getLocalBounds();
            num.setPosition(background.getGlobalBounds().getCenter() - (numBounds.size) / 2.f - numBounds.position);
            target.draw(num, states);
        }

        RootEntity::draw(target, originalStates);
    }

    void update(sf::Time delta) override
    {
        RootEntity::update(delta);

        for (auto& tileRenderer : tileRenderers)
        {
            tileRenderer.updateRenderData();
        }
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

    static Position locationToPosition(Location location)
    {
        return {location.x * blockSize.x, location.y * blockSize.y};
    }

    static Location positionToLocation(Position pos)
    {
        return Location{static_cast<Index>(pos.x / blockSize.x), static_cast<Index>(pos.y / blockSize.y)};
    }

    std::vector<Location> pathfind(Location start, Location end) const
    {
        return generatePath(grid, start, end);
    }
};