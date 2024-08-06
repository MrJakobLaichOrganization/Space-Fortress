#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <box2d/box2d.h>

#include "inputmanager.hpp"
#include "entity.hpp"
#include "time.hpp"
#include "box2d-debug.hpp"

class World
{
  public:
	static constexpr float minZoom = 0.2f;
	static constexpr float maxZoom = 4.f;
	static constexpr float viewSpeed = 100.f;
	sf::Vector2f viewCenter{128.f, 128.f};
	float viewZoom = 1.5f;

	World(sf::RenderWindow &wind, b2Vec2 gravity = b2Vec2{0, 0});
	~World();

	void Update(sf::Time deltaTime);
	void Render(sf::RenderWindow &window);


	Time GetTime() const { return currentTimestamp; }

	void SetDebugDraw(bool on);

	/**
	* @brief Creates an entity in the world
	* @param T - derivation of Entity
	* @param Args - parameters you would put for T
	*/
	template <EntityDerivation T, typename... Args> T &AddEntity(Args &&...args) {
		entities.push_back(std::make_unique<T>(args...));
		return *static_cast<T*>(entities.back().get());
	}

  private:
	b2Vec2 gravity{0, 0};
	std::unique_ptr<b2World> world;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	Time currentTimestamp;
	std::unique_ptr<PhysicsDebugDraw> debugDraw;
	bool drawDebugInfo = false;

	std::vector<std::unique_ptr<Entity>> entities;
};