#pragma once

#include "graphics/fps-counter.hpp"
#include "gui/gui.hpp"
#include "inputmanager.hpp"
#include "world.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>

class Game
{
public:
    sf::Clock clock{};

    InputManager inputManager;

    sf::RenderWindow window{sf::VideoMode({1024, 576}), "My window"};
    World world{window};

    FpsCountrer fpsCounter;

    bool showDebug = false;

    Game()
    {
        window.setVerticalSyncEnabled(true);

        if (!ImGui::SFML::Init(window))
        {
            std::cerr << "Could not initialize ImGui";
            std::exit(-1);
        }
    }

    void run()
    {
        sf::Font fnt = sf::Font::openFromFile(ASSETS_DIR "/IBMPlexMono-Regular.ttf").value();

        MainContainer container{window.getSize()};
        Image& btn = container.addChild<Image>(std::string_view{ASSETS_DIR "/gui/button.png"},
                                               sf::Vector2f{0.5f, 0.5f},
                                               sf::Vector2f{0.5f, 0.5f});
        Text& txt2 = btn.addChild<Text>("test\nasassdf\ni",
                                        fnt,
                                        sf::Vector2f{0.0f, 0.0f},
                                        sf::Vector2f{1.f, 1.f},
                                        TextCentering::LEFT,
                                        TextCentering::DOWN);
        txt2.setText("AAAAA\nGGGGG");
        clock.start();

        while (window.isOpen())
        {
            auto delta = clock.restart();
            if (delta.asSeconds() > 0.1f)
            {
                delta = sf::seconds(0.1f);
            }

            const auto computedViewSpeed = World::viewSpeed * world.viewZoom * delta.asSeconds();

            while (const auto event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                    break;
                }

                ImGui::SFML::ProcessEvent(window, *event);

                if (const auto* e = event->getIf<sf::Event::KeyPressed>())
                {
                    if (!ImGui::GetIO().WantCaptureKeyboard)
                    {
                        inputManager.onKeyPress(e->scancode);
                    }
                }
                else if (const auto* e = event->getIf<sf::Event::KeyReleased>())
                {
                    inputManager.onKeyRelease(e->scancode);
                }
                else if (const auto* e = event->getIf<sf::Event::MouseMoved>())
                {
                    inputManager.screenMousePos = sf::Vector2f(e->position);
                    inputManager.worldMousePos = window.mapPixelToCoords(e->position, world.makeView(window));
                }
                else if (const auto* e = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (e->button == sf::Mouse::Button::Left)
                    {
                        inputManager.leftMouseButonDown = true;
                    }
                    else if (e->button == sf::Mouse::Button::Right)
                    {
                        inputManager.rightMouseButonDown = true;
                    }
                }
                else if (const auto* e = event->getIf<sf::Event::MouseButtonReleased>())
                {
                    if (e->button == sf::Mouse::Button::Left)
                    {
                        inputManager.leftMouseButonDown = false;
                    }
                    else if (e->button == sf::Mouse::Button::Right)
                    {
                        inputManager.rightMouseButonDown = false;
                    }
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
                else if (const auto* e = event->getIf<sf::Event::Resized>())
                {
                    container.onResizeImpl(e->size);
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

            ImGui::SFML::Update(window, delta);

            inputManager.update();
            world.update(delta, inputManager);
            fpsCounter.update(delta);

            container.updateImpl(inputManager);

            window.clear();

            world.render(window);
            window.draw(fpsCounter);

            container.drawImpl(window);

            ImGui::SFML::Render(window);

            window.display();
        }
    }
};