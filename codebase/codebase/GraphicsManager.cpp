#include "GraphicsManager.h"

#include <SDL_image.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Game.h"
#include "FileFunctions.h"

const long double TAU = 2 * M_PI;

GraphicsManager::GraphicsManager()
{
}

GraphicsManager::~GraphicsManager()
{
}

int GraphicsManager::Init()
{
	initState = 0;

	if (initState == 0)
	{
		gameWindow.setTitle("Game Window");
		gameWindow.setPosition({ 100,100,200,200 });
		if (gameWindow.Init() != 0)
		{
			initState = 1;
		}
	}

	if (initState == 0)
	{
		loadAllSpriteSurfaces();
	}

	if (initState == 0)
	{
		loadAllSpriteTextures();
	}

	return initState;
}

int GraphicsManager::Exit()
{
	initState;

	return initState;
}

void GraphicsManager::Events()
{
	gameWindow.Events();
}

void GraphicsManager::Updates()
{
	gameWindow.Updates();
}

void GraphicsManager::Render()
{
	gameWindow.Render();
}

void GraphicsManager::pushWindowToTop(Uint32 windowID)
{
	SDL_RaiseWindow(gameWindow.getSDLWindow());
}

void GraphicsManager::loadAllSpriteSurfaces()
{
	missingTextureSurface = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);
	SDL_FillRect(missingTextureSurface, NULL, SDL_MapRGB(missingTextureSurface->format, 255, 0, 0));

	Game::S().LOGGER.log("Load all Sprite Surfaces (.png format) from disk.");
	std::string filter = ".png";
	std::string spritePath = getFolder("sprites");
	for (std::string pngOnDiskFile : getThingInFolderWithWildcard(spritePath, filter))
	{
		std::string pngOnDiskName = getStemFromPath(pngOnDiskFile);
		SDL_Surface* pngSurface = IMG_Load(pngOnDiskFile.c_str());
		if (pngSurface == NULL)
		{
			Game::S().LOGGER.log("Could not initialize Surface for " + pngOnDiskFile);
			std::string ret_val = IMG_GetError();
			Game::S().LOGGER.log("ERROR: " + ret_val);
		}
		else
		{
			Game::S().LOGGER.log("Initialized Surface " + pngOnDiskFile + " as " + pngOnDiskName + " correctly.");
			spriteSurfaces[pngOnDiskName] = pngSurface;
		}
	}
}

void GraphicsManager::loadAllSpriteTextures()
{
	missingTextureTexture = SDL_CreateTextureFromSurface(gameWindow.getSDLRenderer(), missingTextureSurface);
	if (missingTextureTexture == NULL)
	{
		Game::S().LOGGER.log("Could not initialize Texture from \"MissingTextureSurface\", this should not happen.");
		std::string ret_val = SDL_GetError();
		Game::S().LOGGER.log("ERROR: " + ret_val);
	}

	Game::S().LOGGER.log("Create Textures from Surfaces.");
	for (auto& each : spriteSurfaces)
	{
		std::string spriteSurfaceName = each.first;
		SDL_Surface* spriteSurface = each.second;
		SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(gameWindow.getSDLRenderer(), spriteSurface);
		if (spriteTexture == NULL)
		{
			Game::S().LOGGER.log("Could not initialize Texture from " + spriteSurfaceName + " Surface.");
			std::string ret_val = SDL_GetError();
			Game::S().LOGGER.log("ERROR: " + ret_val);
		}
		else
		{
			Game::S().LOGGER.log("Initialized Texture " + spriteSurfaceName + " correctly.");
			spriteTextures[spriteSurfaceName] = spriteTexture;
		}
	}
}

SDL_Surface* GraphicsManager::getSurface(std::string spriteSurfaceName)
{
	SDL_Surface* returnSurface = missingTextureSurface;
	if (spriteSurfaces.contains(spriteSurfaceName))
	{
		returnSurface = spriteSurfaces[spriteSurfaceName];
	}
	return returnSurface;
}

SDL_Texture* GraphicsManager::getTexture(std::string spriteTextureName)
{
	SDL_Texture* returnTexture = missingTextureTexture;
	if (spriteTextures.contains(spriteTextureName))
	{
		returnTexture = spriteTextures[spriteTextureName];
	}
	return returnTexture;
}

void GraphicsManager::renderTexture(SDL_Texture* spriteTexture, SDL_Rect* SourceRectangle, SDL_Rect* DestinationRectangle)
{
	SDL_RenderCopy(gameWindow.getSDLRenderer(), spriteTexture, SourceRectangle, DestinationRectangle);
}

void GraphicsManager::renderLine(int SourceX, int SourceY, int DestX, int DestY)
{
	SDL_RenderDrawLine(gameWindow.getSDLRenderer(), SourceX, SourceY, DestX, DestY);
}

void GraphicsManager::renderFillBox(SDL_Rect* BoxRect)
{
	SDL_RenderFillRect(gameWindow.getSDLRenderer(), BoxRect);
}

void GraphicsManager::renderCircleBorder(int centerX, int centerY, int range, unsigned int sides)
{
	int cCx = centerX;
	int cCy = centerY;
	int cR = range;
	unsigned int cSides = sides;

	if (cSides < 3)
	{
		cSides = 3;
	}

	float angleIncrement = TAU / cSides;

	int Lx = cCx + cR;
	int Ly = cCy;

	for (int count = 1; count < cSides + 1; count++)
	{
		float angle = count * angleIncrement;

		int x = cCx + cos(angle) * cR;
		int y = cCy + sin(angle) * cR;

		//renderLine(WindowID, cCx, cCy, Lx, Ly);

		renderLine(Lx, Ly, x, y);

		Lx = x;
		Ly = y;
	}
}

void GraphicsManager::renderBoxBorder(SDL_Rect* BoxRect)
{
	SDL_RenderDrawRect(gameWindow.getSDLRenderer(), BoxRect);
}

void GraphicsManager::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(gameWindow.getSDLRenderer(), r, g, b, a);
}

void GraphicsManager::resetDrawColor()
{
	SDL_SetRenderDrawColor(gameWindow.getSDLRenderer(), 255, 255, 255, 255);
}

void GraphicsManager::setViewport(SDL_Rect* rectangle)
{
	SDL_RenderSetViewport(gameWindow.getSDLRenderer(), &rectangle);
}

void GraphicsManager::resetViewport()
{
	SDL_RenderSetViewport(gameWindow.getSDLRenderer(), NULL);
}
