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

	b2Vec2 gravity(0.0f, 10.0f);
	b2World world(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 15.0f);

	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, -4.0f);
	//bodyDef.angle = 2.f;
	b2Body *body = world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f;

	body->CreateFixture(&fixtureDef);

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	Ship ship;

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
		
		world.Step(deltaTime.asSeconds(), velocityIterations, positionIterations);

		ship.Update(deltaTime);

		window.clear();
		window.draw(ship);
		world.DebugDraw();
		window.display();
	}

	return 0;
}