#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <box2d/box2d.h>

#include "inputmanager.hpp"
#include "entity.hpp"

class World
{
  public:
	World(sf::RenderWindow &wind, b2Vec2 gravity = b2Vec2{0, 0});
	~World();

	void Update(sf::RenderWindow &window);
	void Render(sf::RenderWindow &window);

  private:
	static constexpr float minZoom = 0.2f;
	static constexpr float maxZoom = 4.f;
	static constexpr float viewSpeed = 100.f;

	sf::Vector2f viewCenter{128.f, 128.f};
	float viewZoom = 1.5f;

	sf::Clock clock;
	InputManager inputManager;

	b2Vec2 gravity{0, 0};
	b2World world{gravity};
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	std::vector<std::unique_ptr<Entity>> entities;
};