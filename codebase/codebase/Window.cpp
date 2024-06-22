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
		SDLWindow = SDL_CreateWindow(WindowTitle.c_str(), WindowPosition.x, WindowPosition.y, WindowPosition.w, WindowPosition.h, WindowFlags);
		if (SDLWindow == NULL)
		{
			std::string ret_val = SDL_GetError();
			Game::S().LOGGER.log(WindowTitle + " could not be created.");
			Game::S().LOGGER.log("Error: " + ret_val);
			initState = 1;
		}
		else
		{
			Game::S().LOGGER.log("Initialized " + WindowTitle + " successfully.");
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
			Game::S().LOGGER.log("Initialized Renderer successfully.");
			SDL_SetRenderDrawColor(SDLRenderer, 255, 255, 255, 255); // want the window to be white for now
			SDL_SetRenderDrawBlendMode(SDLRenderer, SDL_BLENDMODE_BLEND);
		}
	}

	return initState;
}

int Window::Exit()
{
	initState;

	while (GUIElements.size() > 0)
	{
		// pray to god we never get locked in here because idk how that could even happen lmao
		removeGUIElement(GUIElements[0]->getGUIName());
	}

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

void Window::Events()
{
	for (GUIBaseElement* GUIElement : GUIElements)
	{
		GUIElement->Events();
	}
}

void Window::Updates()
{
	for (GUIBaseElement* GUIElement : GUIElements)
	{
		GUIElement->Updates();
	}
}

void Window::Render()
{
	SDL_RenderClear(SDLRenderer);
	for (GUIBaseElement* GUIElement : GUIElements)
	{
		GUIElement->Render();
	}
	SDL_RenderPresent(SDLRenderer);
}

void Window::SDLEvents(SDL_Event* currentEvent)
{
	switch (currentEvent->window.event)
	{
	case SDL_WINDOWEVENT_CLOSE:
	{
		Game::S().SetRunning(false);
		break;
	}
	case SDL_WINDOWEVENT_SHOWN:
	{
		windowShown = true;
		break;
	}
	case SDL_WINDOWEVENT_HIDDEN:
	{
		windowShown = false;
		break;
	}
	case SDL_WINDOWEVENT_EXPOSED:
	{
		windowExposed = true;
		break;
	}
	case SDL_WINDOWEVENT_MOVED:
	{
		WindowPosition.x = currentEvent->window.data1;
		WindowPosition.y = currentEvent->window.data2;
		break;
	}
	case SDL_WINDOWEVENT_RESIZED:
	{
		// not needed
		// size_changed is more general
		break;
	}
	case SDL_WINDOWEVENT_SIZE_CHANGED:
	{
		WindowPosition.w = currentEvent->window.data1;
		WindowPosition.h = currentEvent->window.data2;
		break;
	}
	case SDL_WINDOWEVENT_MINIMIZED:
	{
		windowState = MINIMIZED;
		break;
	}
	case SDL_WINDOWEVENT_MAXIMIZED:
	{
		windowState = MAXIMIZED;
		break;
	}
	case SDL_WINDOWEVENT_RESTORED:
	{
		windowState = NORMAL;
		break;
	}
	case SDL_WINDOWEVENT_ENTER:
	{
		windowMouseFocus = true;
		Game::S().GRAPHICS.pushWindowToTop(getSDLWindowID());
		break;
	}
	case SDL_WINDOWEVENT_LEAVE:
	{
		windowMouseFocus = false;
		break;
	}
	case SDL_WINDOWEVENT_FOCUS_GAINED:
	{
		windowKeyboardFocus = true;
		break;
	}
	case SDL_WINDOWEVENT_FOCUS_LOST:
	{
		windowKeyboardFocus = false;
		break;
	}
	default:
	{
		break;
	}
	}
}

void Window::setTitle(std::string WT)
{
	WindowTitle = WT;
	SDL_SetWindowTitle(SDLWindow, WindowTitle.c_str());
}

void Window::setPosition(SDL_Rect P)
{
	WindowPosition = P;
	if (WindowPosition.x < 0)
	{
		WindowPosition.x = 0;
	}
	if (WindowPosition.y < 0)
	{
		WindowPosition.y = 0;
	}
	// idk what the min size of a window should be, let's ignore this for now lol
	if (WindowPosition.w < 1)
	{
		WindowPosition.w = 1;
	}
	if (WindowPosition.h < 1)
	{
		WindowPosition.h = 1;
	}
	SDL_SetWindowPosition(SDLWindow, WindowPosition.x, WindowPosition.y);
	SDL_SetWindowSize(SDLWindow, WindowPosition.w, WindowPosition.h);
}

void Window::addGUIElement(GUIBaseElement* GUIElement)
{
	GUIElements.push_back(GUIElement);
}

void Window::removeGUIElement(std::string GUIName)
{
	int index = 0;
	bool found = false;
	for (GUIBaseElement* GUIElement : GUIElements)
	{
		if (GUIElement->getGUIName() == GUIName)
		{
			found = true;
			break;
		}
		index++;
	}
	if (found)
	{
		GUIElements[index]->Exit();
		delete GUIElements[index];
		GUIElements.erase(GUIElements.begin() + index);
	}
}

SDL_Rect Window::scaleRenderRectangleToWindowSize(RenderRectangle& rect)
{
	SDL_Rect scaledRectangle;
	scaledRectangle.x = floor(WindowPosition.w * rect.left);
	scaledRectangle.y = floor(WindowPosition.h * rect.top);
	scaledRectangle.w = ceil(WindowPosition.w * rect.right) - scaledRectangle.x;
	scaledRectangle.h = ceil(WindowPosition.h * rect.bottom) - scaledRectangle.y;
	return scaledRectangle;
}
