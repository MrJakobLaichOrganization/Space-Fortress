#pragma once

#include "box2d-debug.hpp"
#include "entity/entity.hpp"
#include "entity/root-entity.hpp"
#include "graphics/starfield.hpp"
#include "gui/guiwindow.hpp"
#include "time.hpp"
#include "units.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <box2d/b2_math.h>
#include <box2d/b2_types.h>
#include <box2d/b2_world.h>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <vector>

class World
{
public:
    static constexpr float minZoom = 0.2f;
    static constexpr float maxZoom = 4.f;
    static constexpr float viewSpeed = 100.f;
    sf::Vector2f viewCenter{128.f, 128.f};
    float viewZoom = 1.5f;

    World(sf::RenderWindow& wind, b2Vec2 gravity = b2Vec2{0, 0});
    ~World();

    void update(sf::Time deltaTime, class InputManager& inputManager);
    void render(sf::RenderWindow& window);
    void showDebugMenu() const;

    sf::View makeView(const sf::RenderWindow& window) const;

    [[nodiscard]] Time getTime() const
    {
        return m_currentTimestamp;
    }

    void setDebugDraw(bool on);

    /**
	* @brief Creates an entity in the world
	* @param T - derivation of Entity
	* @param Args - parameters you would put for T
	*/
    template <EntityDerivation T, typename... Args>
    T& createEntity(Args&&... args)
    {
        auto entity = std::make_unique<T>(this, m_nextEntityId++, std::forward<Args>(args)...);
        const auto ptr = entity.get();

        m_idToEntity.emplace(entity->id, ptr);
        m_entities.push_back(std::move(entity));

        if constexpr (std::is_base_of_v<RootEntity, T>)
        {
            m_rootEntities.push_back(ptr);
        }

        return *ptr;
    }

    template <typename T = Entity>
    T* findEntity(Entity::Id id)
    {
        auto iter = m_idToEntity.find(id);
        return iter == m_idToEntity.end() ? nullptr : dynamic_cast<T*>(iter->second);
    }

    template <typename T = Entity>
    T& getEntity(Entity::Id id)
    {
        auto ptr = findEntity(id);
        assert(ptr);
        return *ptr;
    }

    template <typename T = Entity>
    const T* findEntity(Entity::Id id) const
    {
        return const_cast<World*>(this)->findEntity(id);
    }

    template <typename T = Entity>
    const T& getEntity(Entity::Id id) const
    {
        return const_cast<World*>(this)->getEntity(id);
    }

    void destroyEntity(Entity::Id id);

    b2World& getPhysicsWorld()
    {
        return *m_world;
    }

    Entity::Id rootEntityUnderMouse{};
    Entity::Id attachEntityUnderMouse{};

    sf::RectangleShape hoverRect;

private:
    void dispatchGUIOrders(class Ship& ship, const Position& pos);

    b2Vec2 m_gravity{0, 0};
    std::unique_ptr<b2World> m_world;
    const int32 m_velocityIterations = 6;
    const int32 m_positionIterations = 2;
    Time m_currentTimestamp;
    std::unique_ptr<PhysicsDebugDraw> m_debugDraw;
    bool m_drawDebugInfo = false;

    std::vector<RootEntity*> m_rootEntities;
    std::unordered_map<Entity::Id, Entity*> m_idToEntity;
    std::vector<std::unique_ptr<Entity>> m_entities;

    Entity::Id m_nextEntityId = 1;

    Starfield m_starfield;

    std::vector<std::unique_ptr<GuiWindow>> m_windows;
};
