#include "GraphicsManager.h"

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
}
