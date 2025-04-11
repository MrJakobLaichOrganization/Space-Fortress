#include "world.hpp"

#include "box2d-utils.hpp"
#include "entity/attach-entities/crewmate.hpp"
#include "entity/attach-entities/thruster.hpp"
#include "entity/root-entities/ship.hpp"
#include "inputmanager.hpp"

#include <algorithm>
#include <exception>
#include <format>
#include <fstream>
#include <imgui-SFML.h>
#include <imgui.h>

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
    firstShip.move({-250.f, 0.f});
    secondShip.move({250.f, 0.f});

    firstShip.rotate(sf::degrees(34.f));

    auto& crewmate = createEntity<Crewmate>("crewmate #1");
    crewmate.move({76.f, 94.f});
    firstShip.attachChild(&crewmate);

    auto& crewmate2 = createEntity<Crewmate>("crewmate #2");
    crewmate2.move({98.f, 94.f});
    firstShip.attachChild(&crewmate2);

    //firstShip.addTileEntity<Thruster>("Thruster", {2, 8}, Direction::Down);
    //firstShip.addTileEntity<Thruster>("Thruster", {7, 7}, Direction::Right);

    for (auto& entity : m_entities)
    {
        if (auto* ship = dynamic_cast<Ship*>(entity.get()))
        {
        }
    }

    //firstShip.body->ApplyForce({1000.f, 0.f}, firstShip.body->GetWorldCenter(), true);

    m_debugDraw = std::make_unique<PhysicsDebugDraw>(&window);
    m_world->SetDebugDraw(m_debugDraw.get());

    m_debugDraw->AppendFlags(
        b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_aabbBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
}

void World::update(sf::Time deltaTime, InputManager& inputManager)
{
    viewZoom = std::clamp(viewZoom, minZoom, maxZoom);

    rootEntityUnderMouse = 0;
    attachEntityUnderMouse = 0;

    for (auto& entity : m_rootEntities)
    {
        entity->prePhysics();
    }

    m_world->Step(deltaTime.asSeconds(), m_velocityIterations, m_positionIterations);

    for (auto& entity : m_rootEntities)
    {
        entity->postPhysics();
    }

    if (const auto bodyUnderMouse = Box2dUtils::findBodyAtPoint(*m_world, toBox2d(inputManager.worldMousePos)))
    {
        if (auto* rootEntity = dynamic_cast<RootEntity*>(bodyUnderMouse->GetUserData().entity))
        {
            rootEntityUnderMouse = rootEntity->id;

            const auto entityLocalMouse = rootEntity->getInverseTransform() * inputManager.worldMousePos;
            for (const auto* child : rootEntity->children)
            {
                auto bounds = child->localBounds;
                bounds.position += child->getPosition();

                if (bounds.contains(entityLocalMouse))
                {
                    attachEntityUnderMouse = child->id;
                    break;
                }
            }
        }
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

    window.setView(makeView(window));
    for (auto& entity : m_rootEntities)
    {
        window.draw(*entity);
    }
    if (m_drawDebugInfo)
    {
        m_world->DebugDraw();
    }

    showDebugMenu();
}

void World::showDebugMenu() const
{
    ImGui::Begin("Debug Menu");

    const auto mousePos = ImGui::GetMousePos();
    const auto mouseText = std::format("x: {} - y: {}", mousePos.x, mousePos.y);
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(mouseText.c_str()).x;
    ImGui::SetCursorPosX(windowWidth - textWidth - 10);
    ImGui::Text("%s", mouseText.c_str());

    if (rootEntityUnderMouse)
    {
        ImGui::Text("%s", std::format("Root Entity: {}", rootEntityUnderMouse).c_str());
    }

    if (attachEntityUnderMouse)
    {
        ImGui::Text("%s", std::format("Attach Entity: {}", attachEntityUnderMouse).c_str());
    }

    ImGui::End();
}

sf::View World::makeView(const sf::RenderWindow& window) const
{
    return {viewCenter, sf::Vector2f(window.getSize()) * viewZoom};
}

void World::destroyEntity(Entity::Id id)
{
    auto iter = m_idToEntity.find(id);
    if (iter == m_idToEntity.end())
    {
        return;
    }

    m_idToEntity.erase(iter);
}

void World::setDebugDraw(bool on)
{
    m_drawDebugInfo = on;
}
