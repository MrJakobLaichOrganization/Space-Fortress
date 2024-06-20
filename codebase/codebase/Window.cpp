#include "Window.h"

#include "Game.h"

Window::Window()
{
}

Window::~Window()
{
}

int Window::Init()
{
	initState = 0;

	if (initState == 0)
	{
		SDLWindow = SDL_CreateWindow(WindowTitle.c_str(), WindowPosX, WindowPosY, WindowSizeX, WindowSizeY, WindowFlags);
		if (SDLWindow == NULL)
		{
			std::string ret_val = SDL_GetError();
			Game::S().LOGGER.log(WindowTitle + " could not be created.");
			Game::S().LOGGER.log("Error: " + ret_val);
			initState = 1;
		}
		else
		{
			Game::S().LOGGER.log("Initializing of " + WindowTitle + " successfully.");
		}
		SDLWindowID = SDL_GetWindowID(SDLWindow);
	}

	if (initState == 0)
	{
		SDLRenderer = SDL_CreateRenderer(SDLWindow, -1, SDL_RENDERER_ACCELERATED);
		if (SDLRenderer == NULL)
		{
			std::string ret_val = SDL_GetError();
			Game::S().LOGGER.log("Renderer could not be created.");
			Game::S().LOGGER.log("Error: " + ret_val);
			initState = 2;
		}
		else
		{
			Game::S().LOGGER.log("Initializing of Renderer successfully.");
			SDL_SetRenderDrawColor(SDLRenderer, 0, 0, 0, 255);
			SDL_SetRenderDrawBlendMode(SDLRenderer, SDL_BLENDMODE_BLEND);
		}
	}

	return 0;
}

int Window::Exit()
{
	initState;

	if (initState >= 2)
	{
		SDL_DestroyRenderer(SDLRenderer);
		SDLRenderer = NULL;
	}
	if (initState >= 1)
	{
		SDL_DestroyWindow(SDLWindow);
		SDLWindow = NULL;
	}

	return initState;
}

void Window::Event()
{
}

void Window::Update()
{
}

void Window::Render()
{
}

void Window::setTitle(std::string WT)
{
	WindowTitle = WT;
	SDL_SetWindowTitle(SDLWindow, WindowTitle.c_str());
}

void Window::setPosition(SDL_Rect P)
{
	WindowPosX = P.x;
	WindowPosY = P.y;
	WindowSizeX = P.w;
	WindowSizeY = P.h;
	if (WindowPosX < 0)
	{
		WindowPosX = 0;
	}
	if (WindowPosY < 0)
	{
		WindowPosY = 0;
	}
	// idk what the min size of a window should be, let's ignore this for now lol
	if (WindowSizeX < 1)
	{
		WindowSizeX = 1;
	}
	if (WindowSizeY < 1)
	{
		WindowSizeY = 1;
	}
	SDL_SetWindowPosition(SDLWindow, WindowPosX, WindowPosY);
	SDL_SetWindowSize(SDLWindow, WindowSizeX, WindowSizeY);
}
