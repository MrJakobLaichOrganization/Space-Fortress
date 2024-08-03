#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>
#include "world.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
	window.setVerticalSyncEnabled(true);

	World world{window};

	while (window.isOpen())
	{
		world.Update(window);

		window.clear();
		world.Render(window);
		window.display();
	}

	return 0;
}