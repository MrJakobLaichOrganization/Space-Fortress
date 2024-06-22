#ifndef __WINDOWCLASS__
#define __WINDOWCLASS__

#include <SDL.h>

#include <string>
#include <vector>

#include "GUIBaseElement.h"

enum WINDOW_STATE
{
	MINIMIZED = -1,
	NORMAL = 0,
	MAXIMIZED = 1
};

class Window
{
public:
	Window();
	~Window();

	int Init();
	int Exit();

	void Events();
	void Updates();
	void Render();

	void SDLEvents(SDL_Event* currentEvent);

	void setTitle(std::string WT);
	std::string getTitle() { return WindowTitle; }
	void setPosition(SDL_Rect P);
	SDL_Rect getPosition() { return WindowPosition; }

	SDL_Window* getSDLWindow() { return SDLWindow; }
	SDL_Renderer* getSDLRenderer() { return SDLRenderer; }
	Uint32 getSDLWindowID() { return SDLWindowID; }

	void addGUIElement(GUIBaseElement* GUIElement);
	void removeGUIElement(std::string GUIName);
	SDL_Rect scaleRenderRectangleToWindowSize(RenderRectangle& rect);
		
private:
	int initState = -1;
	// SDL window specific information
	std::string WindowTitle = "Window";
	SDL_Rect WindowPosition = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 100 };
	Uint32 WindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	// Window information
	bool windowShown = false;
	bool windowExposed = false;
	int windowState = NORMAL;
	bool windowMouseFocus = false;
	bool windowKeyboardFocus = false;
	// SDL native data
	SDL_Window* SDLWindow = NULL;
	SDL_Renderer* SDLRenderer = NULL;
	Uint32 SDLWindowID = 0;
	// GUI data
	std::vector<GUIBaseElement*> GUIElements = {};
};

#endif // !__WINDOWCLASS__
