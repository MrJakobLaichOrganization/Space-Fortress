#include "Game.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

// for testing for now :bedge:
#include "TestGUI.h"

int Game::Init()
{
	// the initState int can be improved with some smartly placed ++ and -- for exit but I'm not gonna bother with that for now
	initState = 0;
	running = false;

	if (initState == 0)
	{
		if (LOGGER.Init() != 0)
		{
			initState = 1;
		}
		else
		{
			LOGGER.log("LoggingManager has been loaded successfully.");
		}
	}

	if (initState == 0)
	{
		if (TIME.Init() != 0)
		{
			LOGGER.log("TimeManager couldn't initialize.");
			initState = 2;
		}
		else
		{
			LOGGER.log("TimeManager has been loaded successfully.");
		}
	}

	if (initState == 0)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			std::string ret_val = SDL_GetError();
			LOGGER.log("SDL didn't load correctly.");
			LOGGER.log("Error: " + ret_val);
			initState = 3;
		}
		else
		{
			LOGGER.log("SDL has been loaded successfully.");
		}
	}

	if (initState == 0)
	{
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			std::string ret_val = IMG_GetError();
			LOGGER.log("SDL_image didn't load correctly.");
			LOGGER.log("Error: " + ret_val);
			initState = 4;
		}
		else
		{
			LOGGER.log("SDL_image has been loaded successfully.");
		}
	}

	if (initState == 0)
	{
		if (TTF_Init() == -1)
		{
			std::string ret_val = TTF_GetError();
			LOGGER.log("SDL_ttf didn't load correctly.");
			LOGGER.log("Error: " + ret_val);
			initState = 5;
		}
		else
		{
			LOGGER.log("SDL_ttf has been loaded successfully.");
		}
	}

	if (initState == 0)
	{
		if (GRAPHICS.Init() != 0)
		{
			LOGGER.log("GraphicsManager couldn't initialize.");
			initState = 6;
		}
		else
		{
			LOGGER.log("GraphicsManager has been loaded successfully.");
		}
	}

	if (initState == 0)
	{
		if (EVENTS.Init() != 0)
		{
			LOGGER.log("EventsManager couldn't initialize.");
			initState = 7;
		}
		else
		{
			LOGGER.log("EventsManager has been loaded successfully.");
		}
	}

	// for testing for now lol
	if (initState == 0)
	{
		TestGUI* testgui = new TestGUI();
		if (testgui->Init() == 0)
		{
			GRAPHICS.getGameWindow().addGUIElement(testgui);
		}
	}

	return initState;
}

int Game::Exit()
{
	running = false; // just to make sure it's not running anymore.

	// because init fail could mean some resources have not been exited out we close out the failed system as well just to be sure.
	if (initState >= 7)
	{
		EVENTS.Exit();
	}
	if (initState >= 6)
	{
		GRAPHICS.Exit();
	}
	if (initState >= 5)
	{
		TTF_Quit();
	}
	if (initState >= 4)
	{
		IMG_Quit();
	}
	if (initState >= 3)
	{
		SDL_Quit();
	}
	if (initState >= 2)
	{
		TIME.Exit();
	}
	if (initState >= 1)
	{
		LOGGER.Exit();
	}

	return initState;
}

int Game::Loop()
{
	if (Init()== 0)
	{
		running = true;
		while (running)
		{
			// here do game loop code stuff
			EVENTS.Events();
			GRAPHICS.Events();
			GRAPHICS.Updates();
			GRAPHICS.Render();

			TIME.Update();
		}
	}

	return Exit();
}

void Game::SetRunning(bool R)
{
	running = R;
}

bool Game::IsRunning()
{
	return running;
}
