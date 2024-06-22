#ifndef __WINDOWCLASS__
#define __WINDOWCLASS__

#include <SDL.h>

#include <string>

class Window
{
public:
	Window();
	~Window();

	int Init();
	int Exit();

	void Event();
	void Update();
	void Render();

	void setTitle(std::string WT);
	std::string getTitle() { return WindowTitle; }
	void setPosition(SDL_Rect P);
	SDL_Rect getPosition() { return { WindowPosX, WindowPosY, WindowSizeX, WindowSizeY }; }

	SDL_Window* getSDLWindow() { return SDLWindow; }
	SDL_Renderer* getSDLRenderer() { return SDLRenderer; }
	Uint32 getSDLWindowID() { return SDLWindowID; }
		
private:
	int initState = -1;
	// SDL window specific information
	std::string WindowTitle = "Window";
	int WindowPosX = SDL_WINDOWPOS_CENTERED;
	int WindowPosY = SDL_WINDOWPOS_CENTERED;
	int WindowSizeX = 100;
	int WindowSizeY = 100;
	Uint32 WindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	// SDL native data
	SDL_Window* SDLWindow = NULL;
	SDL_Renderer* SDLRenderer = NULL;
	Uint32 SDLWindowID = 0;
};

#endif // !__WINDOWCLASS__
