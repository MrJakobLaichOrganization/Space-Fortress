#include <algorithm>

#include <graphics/tilemap.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "block.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
	window.setVerticalSyncEnabled(true);

	auto tileTexture = sf::Texture::loadFromFile(ASSETS_DIR "/TileMap_SF/tileset_sf.png").value();

	Tilemap tilemap(tileTexture, {64, 64}, {32, 32});
	BlockGrid grid({32, 32}, &tilemap);

	const auto floorTile = 10 * 30 + 10;

	grid.SetBlockType(6 * 30 + 10, {0, 0});
	grid.SetBlockType(6 * 30 + 12, {6, 0});

	grid.SetBlockType(8 * 30 + 10, {0, 7});
	grid.SetBlockType(8 * 30 + 12, {6, 7});

	for (std::uint32_t x = 0; x < 5; x++)
	{
		grid.SetBlockType(8 * 30 + 17, {x + 1, 0});
		grid.SetBlockType(9 * 30 + 10, {x + 1, 1});
		grid.SetBlockType(8 * 30 + 17, {x + 1, 7});
		grid.SetBlockType(7 * 30 + 14, {0, x + 1});
		grid.SetBlockType(7 * 30 + 14, {6, x + 1});
		for (std::uint32_t y = 0; y < 5; y++)
		{
			grid.SetBlockType(floorTile, {x + 1, y + 2});
		}
	}

	grid.SetBlockType(7 * 30 + 14, {0, 6});
	grid.SetBlockType(7 * 30 + 14, {6, 6});

	grid.SetBlockType(9 * 30 + 19, {2, 1});

	sf::Vector2f viewCenter{128.f, 128.f};
	float viewZoom = 1.5f;

	constexpr float minZoom = 0.2f;
	constexpr float maxZoom = 4.f;

	constexpr float viewSpeed = 100.f;

	std::array<bool, sf::Keyboard::ScancodeCount> keyStates{};

	sf::Clock clock;

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

		window.clear();
		window.draw(tilemap);
		window.display();
	}

	return 0;
}