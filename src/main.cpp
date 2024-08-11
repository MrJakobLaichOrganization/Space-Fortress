#include "world.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
    window.setVerticalSyncEnabled(true);

    sf::Clock clock{};
    clock.start();

    InputManager inputManager;

    World world{window};

    bool showDebug = false;

    while (window.isOpen())
    {
        const auto delta = clock.restart();
        const auto computedViewSpeed = World::viewSpeed * world.viewZoom * delta.asSeconds();

        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                break;
            }

            if (const auto* e = event->getIf<sf::Event::KeyPressed>())
            {
                inputManager.onKeyPress(e->scancode);
            }
            else if (const auto* e = event->getIf<sf::Event::KeyReleased>())
            {
                inputManager.onKeyRelease(e->scancode);
            }
            else if (const auto* e = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                const auto ratio = e->delta < 0 ? 1.1f : 0.9f;
                const auto newZoom = world.viewZoom * ratio;

                if (newZoom > World::maxZoom || newZoom < World::minZoom)
                {
                    continue;
                }

                const auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                world.viewCenter = mousePos - (mousePos - world.viewCenter) * ratio;
                world.viewZoom = newZoom;
            }
        }

        if (inputManager.isKeyDown(sf::Keyboard::Scan::Left))
        {
            world.viewCenter.x -= computedViewSpeed;
        }
        if (inputManager.isKeyDown(sf::Keyboard::Scan::Right))
        {
            world.viewCenter.x += computedViewSpeed;
        }
        if (inputManager.isKeyDown(sf::Keyboard::Scan::Up))
        {
            world.viewCenter.y -= computedViewSpeed;
        }
        if (inputManager.isKeyDown(sf::Keyboard::Scan::Down))
        {
            world.viewCenter.y += computedViewSpeed;
        }

        if (inputManager.isKeyPressed(sf::Keyboard::Scan::T))
        {
            showDebug = !showDebug;
            world.setDebugDraw(showDebug);
        }

        world.update(delta);
        inputManager.update();

        window.clear();

        world.render(window);

        window.display();
    }

    return 0;
}