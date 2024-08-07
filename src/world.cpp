#include "world.hpp"
#include "ship.hpp"
#include "crewmate.hpp"

#include <algorithm>

namespace
{
std::size_t calculateGridIdx(sf::Vector2<std::size_t> pos, std::size_t width) { return pos.y * width + pos.x; }
void createPolygon(b2Body &body, BlockGrid &grid)
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
}

World::World(sf::RenderWindow &window, b2Vec2 gravity) : m_gravity(gravity)
{
	m_world = std::make_unique<b2World>(m_gravity);
	auto &firstShip = addEntity<Ship>();
	auto &secondShip = addEntity<Ship>();
	firstShip.move({-250.f, 0.f});
	secondShip.move({250.f, 0.f});

	firstShip.rotate(sf::degrees(34.f));

	for (auto &entity : m_entities)
	{
		if (auto *ship = dynamic_cast<Ship *>(entity.get()))
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			ship->body = m_world->CreateBody(&bodyDef);

			/*	
			b2PolygonShape dynamicBox;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
			fixtureDef.restitution = 0.5f;

			for (std::size_t y = 0; y < ship->grid.dims.y; ++y)
			{
				for (std::size_t x = 0; x < ship->grid.dims.x; ++x)
				{
					if (ship->grid.getBlockData(sf::Vector2u(x, y)).blockAchetypeIdx != 0)
					{
						dynamicBox.SetAsBox(1.f / 2.f, 1.f / 2.f, {static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f}, 0.f);
						ship->body->CreateFixture(&fixtureDef);
					}
				}
			}
			*/
			

			createPolygon(*ship->body, ship->grid);
		}
	}

	firstShip.body->ApplyForce({1000.f, 0.f}, firstShip.body->GetWorldCenter(), true);

	m_debugDraw = std::make_unique<PhysicsDebugDraw>(&window);
	m_world->SetDebugDraw(m_debugDraw.get());

	m_debugDraw->AppendFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_aabbBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
}

void World::update(sf::Time deltaTime)
{
	viewZoom = std::clamp(viewZoom, minZoom, maxZoom);

	for (auto &entity : m_entities)
	{
		entity->prePhysics();
	}

	m_world->Step(deltaTime.asSeconds(), m_velocityIterations, m_positionIterations);

	for (auto &entity : m_entities)
	{
		entity->postPhysics();
	}

	for (auto &entity : m_entities)
	{
		entity->update(deltaTime);
	}

	++m_currentTimestamp;
}

void World::render(sf::RenderWindow &window)
{
	window.setView(sf::View(viewCenter, sf::Vector2f(window.getSize()) * viewZoom));
	for (auto &entity : m_entities)
	{
		window.draw(*entity);
	}
	if (m_drawDebugInfo)
	{
		m_world->DebugDraw();
	}
}

void World::setDebugDraw(bool on) { m_drawDebugInfo = on; }
