#pragma once

#include "box2d-debug.hpp"
#include "entity/entity.hpp"
#include "entity/root-entity.hpp"
#include "graphics/starfield.hpp"
#include "inputmanager.hpp"
#include "time.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Window/Event.hpp>

#include <box2d/box2d.h>
#include <utility>

class World
{
public:
    static constexpr float minZoom = 0.2f;
    static constexpr float maxZoom = 4.f;
    static constexpr float viewSpeed = 100.f;
    sf::Vector2f viewCenter{128.f, 128.f};
    float viewZoom = 1.5f;

    World(sf::RenderWindow& wind, b2Vec2 gravity = b2Vec2{0, 0});
    ~World() = default;

    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);


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

    b2World& getPhysicsWorld()
    {
        return *m_world;
    }

private:
    b2Vec2 m_gravity{0, 0};
    std::unique_ptr<b2World> m_world;
    const int32 m_velocityIterations = 6;
    const int32 m_positionIterations = 2;
    Time m_currentTimestamp;
    std::unique_ptr<PhysicsDebugDraw> m_debugDraw;
    bool m_drawDebugInfo = false;

    std::vector<RootEntity*> m_rootEntities;
    std::vector<std::unique_ptr<Entity>> m_entities;
    std::unordered_map<Entity::Id, Entity*> m_idToEntity;

    Entity::Id m_nextEntityId{};

    Starfield starfield;
};
