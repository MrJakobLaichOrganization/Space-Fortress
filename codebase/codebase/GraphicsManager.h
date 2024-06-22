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

	void Event();
	void Update();
	void Render();

	void loadAllSpriteSurfaces();
	void loadAllSpriteTextures();

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
