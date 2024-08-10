#include "world.hpp"

#include "crewmate.hpp"
#include "ship.hpp"
#include "block.hpp"

#include <algorithm>
#include <exception>
#include <fstream>

#include <cereal/archives/json.hpp>

namespace
{
std::size_t calculateGridIdx(sf::Vector2<std::size_t> pos, std::size_t width)
{
    return pos.y * width + pos.x;
}
void createPolygon(b2Body& body, BlockGrid& grid)
{
    std::vector<std::uint8_t> solidTiles(grid.dims.y * grid.dims.x, 0);

    b2PolygonShape dynamicBox;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;

    // Fill the solid tiles
    for (std::size_t y = 0; y < grid.dims.y; ++y)
    {
        for (std::size_t x = 0; x < grid.dims.x; ++x)
        {
            solidTiles[calculateGridIdx({x, y}, grid.dims.x)] = grid.getBlockArchetype(sf::Vector2u(x, y)).solid;
        }
    }

    // Create the box strips
    for (std::size_t y = 0; y < grid.dims.y; ++y)
    {
        for (std::size_t x = 0; x < grid.dims.x; ++x)
        {
            if (!solidTiles[calculateGridIdx({x, y}, grid.dims.x)])
            {
                continue;
            }
            solidTiles[calculateGridIdx({x, y}, grid.dims.x)] = 0;

            sf::Vector2<std::size_t> leftPos;
            leftPos.x = x;
            leftPos.y = y;
            sf::Vector2<std::size_t> rightPos;
            rightPos.x = x + 1;
            rightPos.y = y;

            // Go left
            while (leftPos.x != 0 && leftPos.x - 1 > 0)
            {
                const std::size_t idx = calculateGridIdx({leftPos}, grid.dims.x);
                if (idx >= solidTiles.size() || !solidTiles[idx])
                {
                    break;
                }

                leftPos.x--;
                solidTiles[calculateGridIdx(leftPos, grid.dims.x)] = 0;
            }

            // Go right
            do
            {
                const std::size_t idx = calculateGridIdx({rightPos}, grid.dims.x);
                if (idx >= solidTiles.size() || !solidTiles[idx])
                {
                    break;
                }

                solidTiles[calculateGridIdx(rightPos, grid.dims.x)] = 0;
                rightPos.x++;
            } while (rightPos.x <= grid.dims.x);

            // If no movement done, move vertically
            if (leftPos == sf::Vector2<std::size_t>{rightPos.x - 1, rightPos.y})
            {
                rightPos.x -= 1;
                rightPos.y++;
                // Go up
                while (leftPos.y > 0)
                {
                    const std::size_t idx = calculateGridIdx({leftPos}, grid.dims.x);
                    if (idx >= solidTiles.size() || !solidTiles[idx])
                    {
                        break;
                    }
                    solidTiles[calculateGridIdx(leftPos, grid.dims.x)] = 0;
                    leftPos.y--;
                }

                // Go down
                do
                {
                    const std::size_t idx = calculateGridIdx({rightPos}, grid.dims.x);
                    if (idx >= solidTiles.size() || !solidTiles[idx])
                    {
                        break;
                    }
                    solidTiles[calculateGridIdx(rightPos, grid.dims.x)] = 0;
                    rightPos.y++;
                } while (rightPos.x < grid.dims.y);

                // If no bigger box still, make one small
                if (leftPos == sf::Vector2<std::size_t>{rightPos.x, rightPos.y - 1})
                {
                    dynamicBox.SetAsBox(1.f / 2.f, 1.f / 2.f, {x + 0.5f, y + 0.5f}, 0.0f);
                    body.CreateFixture(&fixtureDef);
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
            body.CreateFixture(&fixtureDef);
        }
    }
}
} // namespace

World::World(sf::RenderWindow& window, b2Vec2 gravity) : m_gravity(gravity)
{
    {
        // Load the block grid archetypes
        std::ifstream file(CONFIG_DIR "/block_types.json");
        if (!file.is_open())
        {
            throw std::ios_base::failure("Couldn't open '" CONFIG_DIR "/block_types.json'");
        }
        cereal::JSONInputArchive archive{file};
        BlockGrid::loadArchetypes(archive);
        file.close();
    }

    m_world = std::make_unique<b2World>(m_gravity);
    auto& firstShip = createEntity<Ship>();
    auto& secondShip = createEntity<Ship>();
    firstShip.move({-250.f, 0.f});
    secondShip.move({250.f, 0.f});

    firstShip.rotate(sf::degrees(34.f));

    auto& crewmate = createEntity<Crewmate>(*this, "crewmate #1");

    crewmate.move({76.f, 94.f});
    firstShip.attachChild(&crewmate);

    crewmate.setMoveTarget({2, 2});

    for (auto& entity : m_entities)
    {
        if (auto* ship = dynamic_cast<Ship*>(entity.get()))
        {
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            ship->body = m_world->CreateBody(&bodyDef);

            createPolygon(*ship->body, ship->grid);
        }
    }

    firstShip.body->ApplyForce({1000.f, 0.f}, firstShip.body->GetWorldCenter(), true);

    m_debugDraw = std::make_unique<PhysicsDebugDraw>(&window);
    m_world->SetDebugDraw(m_debugDraw.get());

    m_debugDraw->AppendFlags(
        b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_aabbBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
}

void World::update(sf::Time deltaTime)
{
    viewZoom = std::clamp(viewZoom, minZoom, maxZoom);

    for (auto& entity : m_rootEntities)
    {
        entity->prePhysics();
    }

    m_world->Step(deltaTime.asSeconds(), m_velocityIterations, m_positionIterations);

    for (auto& entity : m_rootEntities)
    {
        entity->postPhysics();
    }

    for (auto& entity : m_rootEntities)
    {
        entity->update(deltaTime);
    }

    ++m_currentTimestamp;
}

void World::render(sf::RenderWindow& window)
{
    window.setView(sf::View(viewCenter, sf::Vector2f(window.getSize()) * viewZoom));
    for (auto& entity : m_rootEntities)
    {
        window.draw(*entity);
    }
    if (m_drawDebugInfo)
    {
        m_world->DebugDraw();
    }
}

void World::setDebugDraw(bool on)
{
    m_drawDebugInfo = on;
}

// pathing
namespace
{
struct PathNode
{
    PathNode* parent;
    sf::Vector2i pos;
    float cost = std::numeric_limits<float>::max();

    bool operator==(const PathNode& node)
    {
        return node.pos == pos;
    }
    bool operator==(const sf::Vector2i& position)
    {
        return pos == position;
    }
};

float getTileValue(sf::Vector2i pos, sf::Vector2i target)
{
    return std::sqrtf(pos.x - target.x) + std::sqrtf(pos.y - target.y);
}

std::queue<sf::Vector2i> retracePath(const PathNode* start, const PathNode* end)
{
    const PathNode* tmpNode = end;
    std::vector<sf::Vector2i> tmpPath{};
    std::queue<sf::Vector2i> path{};

    while (tmpNode != start)
    {
        tmpPath.push_back(tmpNode->pos - tmpNode->parent->pos);
        tmpNode = tmpNode->parent;
    }
    for (auto it = tmpPath.rbegin(); it != tmpPath.rend(); it++)
    {
        path.push(*it);
    }

    return path;
}
} // namespace

std::queue<sf::Vector2i> World::generatePath(const BlockGrid& grid, sf::Vector2i start, sf::Vector2i end, std::size_t maxSteps) const
{
    std::deque<PathNode> openTiles{};
    std::list<PathNode> traveledTiles{};
    static const std::array<sf::Vector2i, 4> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
    auto isSolid = [&grid](sf::Vector2i pos)
    { return grid.getBlockArchetype(grid.getBlockData(sf::Vector2u(pos.x, pos.y)).blockAchetypeIdx).solid; };
    auto isValid = [&grid, &isSolid](sf::Vector2i pos) { return pos.x >= 0 && pos.y >= 0 && !isSolid(pos); };

    traveledTiles.push_back({nullptr, start, 0});

    // populate the directional tiles
    for (const auto& dir : directions)
    {
        auto position = start + dir;
        if (!isValid(position))
            continue;

        openTiles.push_back(PathNode{&traveledTiles.front(),
                                     position, getTileValue(position,
                                                  end)});
    }

    auto pos = openTiles.front().pos;
    while (maxSteps-- && openTiles.size())
    {
        std::size_t minIdx = 0;
        for (std::size_t i = 1; i < openTiles.size(); ++i)
        {
            if (openTiles[i].cost < openTiles[minIdx].cost)
            {
                minIdx = i;
            }
        }

        traveledTiles.push_back(openTiles[minIdx]);
        pos = openTiles[minIdx].pos;
        openTiles.erase(openTiles.begin() + minIdx);

        if (pos == end)
        {
            return retracePath(&traveledTiles.front(), &traveledTiles.back());
        }

        for (const auto& dir : directions)
        {
            if (pos.x == 0 && dir.x < 0 || pos.y == 0 && dir.y < 0)
            {
                continue;
            }
            if (!isValid(pos + dir) ||
                std::find(traveledTiles.begin(), traveledTiles.end(), pos + dir) != traveledTiles.end())
            {
                continue;
            }
            if (std::find(openTiles.begin(), openTiles.end(), pos + dir) != openTiles.end())
            {
                continue;
            }
            openTiles.push_back(PathNode{&traveledTiles.back(), {pos + dir}, getTileValue(pos + dir, end)});
        }
    }

    return {};
}