#include "TestGUI.h"

#include "Game.h"

TestGUI::TestGUI()
{
}

TestGUI::~TestGUI()
{
}

int TestGUI::Init()
{
	initState = 0;

	if (initState == 0)
	{
		testTexture = Game::S().GRAPHICS.getTexture("grass");
	}

	if (initState == 0)
	{
		// for testing I make this only a partial size of the full window
		inWindowRect = { 0.25f, 0.5f, 0.9f, 0.75f };
	}

	return initState;
}
 
int TestGUI::Exit()
{
	initState;

	return initState;
}

void TestGUI::Events()
{
}

void TestGUI::Updates()
{
}

void TestGUI::Render()
{
	/*
	* default GUI behavior:
	* scale its inWindowRect to the window size
	* then create the viewport
	* then scale every coordinate to the usedRect size and render to there
	*/
	SDL_Rect scaledRect = Game::S().GRAPHICS.getGameWindow().scaleRenderRectangleToWindowSize(inWindowRect);
	Game::S().GRAPHICS.setViewport(&scaledRect);
	SDL_Rect usedRect = { 0, 0, scaledRect.w, scaledRect.h };

	Game::S().GRAPHICS.renderTexture(testTexture, NULL, &usedRect);

	// here every element is scaled to the coordinates of usedRect and then rendered

	Game::S().GRAPHICS.resetViewport();
}
