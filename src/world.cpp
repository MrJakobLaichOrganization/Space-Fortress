#include "world.hpp"

#include "crewmate.hpp"
#include "ship.hpp"

#include <algorithm>

namespace
{
std::size_t calculateGridIdx(sf::Vector2<std::size_t> pos, std::size_t width)
{
    return pos.y * width + pos.x;
}
void createPolygon(b2Body& body, BlockGrid& grid)
{
    std::vector<b2Vec2> hull;

    const auto blockSize = toBox2d({64.f, 64.f});

    const auto isBlocked = [&](sf::Vector2i pos)
    {
        if (pos.x < 0 || pos.y < 0 || pos.x >= grid.dims.x || pos.y >= grid.dims.y)
        {
            return false;
        }

        return grid.getBlockArchetype(sf::Vector2u{pos}).solid;
    };

    const auto canMove = [&](sf::Vector2i pos, int d)
    {
        if (d == 0)
        {
            return isBlocked({pos.x, pos.y - 1}) && !isBlocked({pos.x - 1, pos.y - 1});
        }
        else if (d == 1)
        {
            return isBlocked({pos.x, pos.y}) && !isBlocked({pos.x, pos.y - 1});
        }
        else if (d == 2)
        {
            return !isBlocked({pos.x, pos.y}) && isBlocked({pos.x - 1, pos.y});
        }
        else if (d == 3)
        {
            return isBlocked({pos.x - 1, pos.y - 1}) && !isBlocked({pos.x - 1, pos.y});
        }
    };

    const auto startBlock = [&]()
    {
        for (int y = 0; y < grid.dims.y; ++y)
        {
            for (int x = 0; x < grid.dims.x; ++x)
            {
                if (isBlocked({x, y}))
                {
                    return sf::Vector2i{x, y};
                }
            }
        }
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

        hull.push_back({currentBlock.x * blockSize.x, currentBlock.y * blockSize.y});
        currentBlock += dirs[dir];
    } while (currentBlock != startBlock);


    b2PolygonShape dynamicBox;
    dynamicBox.Set(hull.data(), hull.size());

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;

    body.CreateFixture(&fixtureDef);
}
} // namespace

World::World(sf::RenderWindow& window, b2Vec2 gravity) : m_gravity(gravity)
{
    m_world = std::make_unique<b2World>(m_gravity);
    auto& firstShip = addEntity<Ship>();
    auto& secondShip = addEntity<Ship>();
    firstShip.move({-250.f, 0.f});
    secondShip.move({250.f, 0.f});

    firstShip.rotate(sf::degrees(34.f));

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

    for (auto& entity : m_entities)
    {
        entity->prePhysics();
    }

    m_world->Step(deltaTime.asSeconds(), m_velocityIterations, m_positionIterations);

    for (auto& entity : m_entities)
    {
        entity->postPhysics();
    }

    for (auto& entity : m_entities)
    {
        entity->update(deltaTime);
    }

    ++m_currentTimestamp;
}

void World::render(sf::RenderWindow& window)
{
    window.setView(sf::View(viewCenter, sf::Vector2f(window.getSize()) * viewZoom));
    for (auto& entity : m_entities)
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
