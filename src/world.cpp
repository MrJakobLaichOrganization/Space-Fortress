#include "world.hpp"
#include "box2d-debug.hpp"
#include "ship.hpp"

World::World(sf::RenderWindow &window, b2Vec2 gravity_) : gravity(gravity_)
{
	world = std::make_unique<b2World>(gravity);
	entities.push_back(std::make_unique<Ship>());
	entities.push_back(std::make_unique<Ship>());
	entities[0]->move({-250.f, 0.f});
	entities[1]->move({250.f, 0.f});

	entities[0]->rotate(sf::degrees(34.f));

	for (auto &entity : entities)
	{
		if (auto *ship = dynamic_cast<Ship *>(entity.get()))
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			ship->body = world->CreateBody(&bodyDef);

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
					if (ship->grid.GetBlockData(sf::Vector2u(x, y)).blockAchetypeIdx != 0)
					{
						dynamicBox.SetAsBox(1.f / 2.f, 1.f / 2.f, {static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f}, 0.f);
						ship->body->CreateFixture(&fixtureDef);
					}
				}
			}
		}
	}

	entities[0]->body->ApplyForce({1000.f, 0.f}, entities[0]->body->GetWorldCenter(), true);

	PhysicsDebugDraw debugDraw{&window};
	world->SetDebugDraw(&debugDraw);

	debugDraw.AppendFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_aabbBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
}
World::~World() {}

void World::Update(const sf::Time &deltaTime)
{
	viewZoom = std::clamp(viewZoom, minZoom, maxZoom);

	for (auto &entity : entities)
	{
		entity->PrePhysics();
	}

	world->Step(deltaTime.asSeconds(), velocityIterations, positionIterations);

	for (auto &entity : entities)
	{
		entity->PostPhysics();
	}

	for (auto &entity : entities)
	{
		entity->Update(deltaTime);
	}
}

void World::Render(sf::RenderWindow &window)
{
	window.setView(sf::View(viewCenter, sf::Vector2f(window.getSize()) * viewZoom));
	for (auto &entity : entities)
	{
		window.draw(*entity);
	}
}