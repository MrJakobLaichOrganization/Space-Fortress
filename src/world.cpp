#include "world.hpp"

#include "entity/attach-entities/crewmate.hpp"
#include "entity/attach-entities/thruster.hpp"
#include "entity/root-entities/ship.hpp"

#include <algorithm>
#include <exception>
#include <fstream>

#include <cereal/archives/json.hpp>

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
    firstShip.move({-150.f, 0.f});
    secondShip.move({250.f, 0.f});

    for (int x = 0; x < 00; x++)
    {
        auto& ship = createEntity<Ship>();
        ship.move({(x / 10) * 300.f, (x % 10) * 500.f});
        ship.addMachine<Thruster>({2, 8}, Direction::Down);
    }

    firstShip.rotate(sf::degrees(34.f));

    auto& crewmate = createEntity<Crewmate>("crewmate #1");
    crewmate.move({76.f, 94.f});
    firstShip.attachChild(&crewmate);

    //firstShip.addMachine<Thruster>({2, 8}, Direction::Down);
    //firstShip.addMachine<Thruster>({7, 7}, Direction::Right);

    for (auto& entity : m_entities)
    {
        if (auto* ship = dynamic_cast<Ship*>(entity.get()))
        {
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

    for (int x = 0; x < 1; x++)
        m_world->Step(deltaTime.asSeconds(), m_velocityIterations, m_positionIterations);

    for (auto& entity : m_rootEntities)
    {
        entity->postPhysics();
    }

    for (auto& entity : m_rootEntities)
    {
        entity->update(deltaTime);
    }

    m_starfield.update(deltaTime, viewCenter, viewZoom);

    ++m_currentTimestamp;
}

void World::render(sf::RenderWindow& window)
{
    m_starfield.draw(window, {});

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
