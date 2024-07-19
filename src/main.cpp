#include <graphics/tilemap.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "block.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

	Tilemap tilemap("tilesheet.png", {64, 64}, {12, 9});
	BlockGrid grid({12, 9}, &tilemap);

	grid.SetBlockType(1, {1, 2});

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		window.clear();
		window.draw(tilemap);
		window.display();
	}

	return 0;
}