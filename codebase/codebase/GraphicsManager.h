#ifndef __GRAPHICSMANAGER__
#define __GRAPHICSMANAGER__

#include <SDL.h>

#include <string>
#include <map>

#include "Window.h"

class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();

	int Init();
	int Exit();

	void Events();
	void Updates();
	void Render();

	Window& getGameWindow() { return gameWindow; }
	void pushWindowToTop(Uint32 windowID);

	void loadAllSpriteSurfaces();
	void loadAllSpriteTextures();

	SDL_Surface* getSurface(std::string spriteSurfaceName);
	SDL_Texture* getTexture(std::string spriteTextureName);

	// we don't render surfaces, that's CPU stuff and I won't stand for it :bedge:
	void renderTexture(SDL_Texture* spriteTexture, SDL_Rect* SourceRectangle = NULL, SDL_Rect* DestinationRectangle = NULL);

	void renderLine(int, int, int, int);
	void renderBoxBorder(SDL_Rect*);
	void renderFillBox(SDL_Rect*);
	void renderCircleBorder(int, int, int, unsigned int = 3);

	void setDrawColor(Uint8, Uint8, Uint8, Uint8 = 255);
	void resetDrawColor();

	void setViewport(SDL_Rect*);
	void resetViewport();

private:
	int initState = -1;
	// Window data
	Window gameWindow;

	SDL_Surface* missingTextureSurface = NULL;
	SDL_Texture* missingTextureTexture = NULL;
	std::map<std::string, SDL_Surface*> spriteSurfaces;
	std::map<std::string, SDL_Texture*> spriteTextures;
};

#endif // !__GRAPHICMANAGER__
