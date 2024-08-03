#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <box2d/box2d.h>

#include "block.hpp"
#include "entity.hpp"
#include "ship.hpp"

#include "box2d-debug.hpp"

#include "graphics/tilemap.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
	window.setVerticalSyncEnabled(true);

	sf::Vector2f viewCenter{128.f, 128.f};
	float viewZoom = 1.5f;

	constexpr float minZoom = 0.2f;
	constexpr float maxZoom = 4.f;

	constexpr float viewSpeed = 100.f;

	std::array<bool, sf::Keyboard::ScancodeCount> keyStates{};

	sf::Clock clock;

	b2Vec2 gravity(0.0f, 0.0f);
	b2World world(gravity);

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	std::vector<std::unique_ptr<Entity>> entities;

	entities.push_back(std::make_unique<Ship>());
	entities.push_back(std::make_unique<Ship>());

	entities[0]->move({-250.f, 0.f});
	entities[1]->move({250.f, 0.f});
	
	entities[0]->rotate(sf::degrees(34.f));

	for (auto& entity : entities)
	{
		if (auto* ship = dynamic_cast<Ship*>(entity.get()))
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			ship->body = world.CreateBody(&bodyDef);

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
					if (ship->grid.GetBlockData(sf::Vector2u( x, y )).blockAchetypeIdx != 0)
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
	world.SetDebugDraw(&debugDraw);

	debugDraw.AppendFlags(
        b2Draw::e_shapeBit |
        b2Draw::e_jointBit |
        b2Draw::e_aabbBit |
        b2Draw::e_pairBit |
        b2Draw::e_centerOfMassBit);

	while (window.isOpen())
	{
		const auto deltaTime = clock.restart();
		while (const auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			else if (const auto* e = event->getIf<sf::Event::KeyPressed>())
			{
				keyStates[static_cast<std::size_t>(e->scancode)] = true;
			}
			else if (const auto *e = event->getIf<sf::Event::KeyReleased>())
			{
				keyStates[static_cast<std::size_t>(e->scancode)] = false;
			}
			else if (const auto* e = event->getIf<sf::Event::MouseWheelScrolled>())
			{
				const auto ratio = e->delta < 0 ? 1.1f : 0.9f;
				const auto newZoom = viewZoom * ratio;

				if (newZoom > maxZoom || newZoom < minZoom)
				{
					continue;
				}

				const auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				viewCenter = mousePos - (mousePos - viewCenter) * ratio;
				viewZoom = newZoom;
			}
		}

		const auto computedViewSpeed = viewSpeed * viewZoom * deltaTime.asSeconds();
		if (keyStates[static_cast<std::size_t>(sf::Keyboard::Scan::Left)])
		{
			viewCenter.x -= computedViewSpeed;
		}
		if (keyStates[static_cast<std::size_t>(sf::Keyboard::Scan::Right)])
		{
			viewCenter.x += computedViewSpeed;
		}
		if (keyStates[static_cast<std::size_t>(sf::Keyboard::Scan::Up)])
		{
			viewCenter.y -= computedViewSpeed;
		}
		if (keyStates[static_cast<std::size_t>(sf::Keyboard::Scan::Down)])
		{
			viewCenter.y += computedViewSpeed;
		}

		viewZoom = std::clamp(viewZoom, 0.2f, 4.f);
		window.setView(sf::View(viewCenter, sf::Vector2f(window.getSize()) * viewZoom));
		
		for (auto& entity : entities)
		{
			entity->PrePhysics();
		}

		world.Step(deltaTime.asSeconds(), velocityIterations, positionIterations);

		for (auto& entity : entities)
		{
			entity->PostPhysics();
		}

		for (auto& entity : entities)
		{
			entity->Update(deltaTime);
		}

		window.clear();

		for (auto& entity : entities)
		{
			window.draw(*entity);
		}

		//world.DebugDraw();
		window.display();
	}

	return 0;
}