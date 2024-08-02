#include <graphics/tilemap.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "block.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");

	auto tileTexture = sf::Texture::loadFromFile(ASSETS_DIR "/TileMap_SF/tileset_sf.png").value();

	Tilemap tilemap(tileTexture, {64, 64}, {12, 9});
	BlockGrid grid({12, 9}, &tilemap);

	grid.SetBlockType(1, {1, 2});

	while (window.isOpen())
	{
		while (const auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		window.clear();
		window.draw(tilemap);
		window.display();
	}

	return 0;
}