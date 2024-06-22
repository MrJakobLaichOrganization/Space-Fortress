#include "GraphicsManager.h"

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
	std::string spritePath = getFolder("sprites"); // check which path this is

	//std::string graphicpath = ConfigManager::S()->DATA["SETTINGS"]["graphicpath"];
	//for (std::string windowFolders : listThingInFolder(graphicpath))
	//{
	//	std::string windowFolderStem = getStemFromPath(windowFolders);
	//	Game::S()->log("Loading Surfaces from window folder \"" + windowFolderStem + "\"");
	//	// todo
	//	// instead of loading all files in this folder I think I could just recursively load all files from all folders from inside each window folder
	//	// in case I later get fucked by "too many images in one folder" stupid stuff
	//	for (std::string thingInWindowFolder : listThingInFolder(graphicpath + "/" + windowFolderStem, filter))
	//	{
	//		std::string spriteStem = getStemFromPath(thingInWindowFolder);
	//		SpriteSurfaces[windowFolderStem][spriteStem] = IMG_Load(thingInWindowFolder.c_str());
	//		if (SpriteSurfaces[windowFolderStem][spriteStem] == NULL)
	//		{
	//			Game::S()->log("Could not initialize Surface for " + thingInWindowFolder);
	//			std::string ret_val = IMG_GetError();
	//			Game::S()->log("Error: " + ret_val);
	//			SpriteSurfaces[windowFolderStem].erase(spriteStem);
	//			abort = true;
	//			break;
	//		}
	//		else
	//		{
	//			Game::S()->log("Initialized Surface " + thingInWindowFolder + " correctly.");
	//		}
	//	}

	//	if (abort == true)
	//	{
	//		break;
	//	}
	//}

}

void GraphicsManager::loadAllSpriteTextures()
{
}
