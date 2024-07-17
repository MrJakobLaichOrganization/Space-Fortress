#include <graphics/tilemap/tilemap.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

	Tilemap tilemap("test.png", {8, 8}, {800, 600});
	tilemap.SetTile({4, 4}, 3);

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
			case sf::Event::Resized:
				tilemap.Resize({event.size.width, event.size.height});
				break;
			}
		}

		window.clear();
		window.draw(tilemap);
		window.display();
	}

	return 0;
}