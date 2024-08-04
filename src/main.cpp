#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>
#include "world.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
	window.setVerticalSyncEnabled(true);
	sf::Clock clock;
	InputManager inputManager;
	clock.start();

	World world{window};

	bool showDebug = false;

	while (window.isOpen())
	{
		const auto delta = clock.restart();
		const auto computedViewSpeed = world.viewSpeed * world.viewZoom * delta.asSeconds();

		while (const auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
				break;
			}
			else if (const auto *e = event->getIf<sf::Event::KeyPressed>())
			{
				inputManager.OnKeyPress(e->scancode);
			}
			else if (const auto *e = event->getIf<sf::Event::KeyReleased>())
			{
				inputManager.OnKeyRelease(e->scancode);
			}
			else if (const auto *e = event->getIf<sf::Event::MouseWheelScrolled>())
			{
				const auto ratio = e->delta < 0 ? 1.1f : 0.9f;
				const auto newZoom = world.viewZoom * ratio;

				if (newZoom > world.maxZoom || newZoom < world.minZoom)
				{
					continue;
				}

				const auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				world.viewCenter = mousePos - (mousePos - world.viewCenter) * ratio;
				world.viewZoom = newZoom;
			}
		}

		if (inputManager.IsKeyDown(sf::Keyboard::Scan::Left))
		{
			world.viewCenter.x -= computedViewSpeed;
		}
		if (inputManager.IsKeyDown(sf::Keyboard::Scan::Right))
		{
			world.viewCenter.x += computedViewSpeed;
		}
		if (inputManager.IsKeyDown(sf::Keyboard::Scan::Up))
		{
			world.viewCenter.y -= computedViewSpeed;
		}
		if (inputManager.IsKeyDown(sf::Keyboard::Scan::Down))
		{
			world.viewCenter.y += computedViewSpeed;
		}

		world.Update(delta);
		inputManager.Update();

		window.clear();
		world.Render(window);
		window.display();
	}

	return 0;
}