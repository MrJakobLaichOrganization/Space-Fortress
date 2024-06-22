#include "GraphicsManager.h"

#include <SDL_image.h>

#include "Game.h"
#include "FileFunctions.h"

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

void GraphicsManager::Event()
{
}

void GraphicsManager::Update()
{
}

void GraphicsManager::Render()
{
	gameWindow.Render();
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
			spriteSurfaces[pngOnDiskFile] = pngSurface;
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
