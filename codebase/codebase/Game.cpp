#include "Game.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

int Game::Init()
{
	initState = 0;
	running = false;

	if (initState == 0)
	{
		if (LOGGER.Init() != 0)
		{
			initState = 1;
		}
	}

	if (initState == 0)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			//LAUNCH_DATA["LOADING_STATE"]["SDL_INIT_SUCCESSFULL"] = false;
			std::string ret_val = SDL_GetError();
			LOGGER.log("SDL didn't load correctly.");
			LOGGER.log("Error: " + ret_val);
			initState = 2;
		}
		else
		{
			//LAUNCH_DATA["LOADING_STATE"]["SDL_INIT_SUCCESSFULL"] = true;
			LOGGER.log("SDL has been loaded successfully.");
		}
	}

	if (initState == 0)
	{
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			//LAUNCH_DATA["LOADING_STATE"]["SDL_INIT_SUCCESSFULL"] = false;
			std::string ret_val = IMG_GetError();
			LOGGER.log("SDL_image didn't load correctly.");
			LOGGER.log("Error: " + ret_val);
			initState = 3;
		}
		else
		{
			//LAUNCH_DATA["LOADING_STATE"]["SDL_IMAGE_INIT_COUNT"] = true;
			LOGGER.log("SDL_image has been loaded successfully.");
		}
	}

	if (initState == 0)
	{
		if (TTF_Init() == -1)
		{
			//LAUNCH_DATA["LOADING_STATE"]["SDL_INIT_SUCCESSFULL"] = false;
			std::string ret_val = TTF_GetError();
			LOGGER.log("SDL_ttf didn't load correctly.");
			LOGGER.log("Error: " + ret_val);
			initState = 4;
		}
		else
		{
			//LAUNCH_DATA["LOADING_STATE"]["SDL_TTF_INIT_COUNT"] = true;
			LOGGER.log("SDL_ttf has been loaded successfully.");
		}
	}

	if (initState == 0)
	{
		if (GRAPHICS.Init() != 0)
		{
			LOGGER.log("GraphicsManager couldn't initialize.");
			initState = 5;
		}
	}

	if (initState == 0)
	{
		if (EVENTS.Init() != 0)
		{
			LOGGER.log("EventsManager couldn't initialize.");
			initState = 6;
		}
	}

	return initState;
}

int Game::Exit()
{
	running = false; // just to make sure it's not running anymore.

	// because init fail could mean some resources have not been exited out we close out the failed system as well just to be sure.
	if (initState >= 6)
	{
		EVENTS.Exit();
	}
	if (initState >= 5)
	{
		GRAPHICS.Exit();
	}
	if (initState >= 4)
	{
		TTF_Quit();
	}
	if (initState >= 3)
	{
		IMG_Quit();
	}
	if (initState >= 2)
	{
		SDL_Quit();
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
