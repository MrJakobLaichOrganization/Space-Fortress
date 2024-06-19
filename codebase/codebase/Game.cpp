#include "Game.h"

int Game::Init()
{
	initState = -1;
	running = false;

	return initState;
}

int Game::Exit()
{
	exitState = -3;

	switch (initState)
	{
	case -1:
		// default error case
		break;
	case 0:
		// first step worked, etc
	default:
		// this should never be hit!
		break;
	}

	return exitState;
}

int Game::Loop()
{
	if (Init()== 0)
	{
		running = true;
		while (running)
		{
			// here do game loop code stuff
			running = false;
		}
	}

	return Exit();
}

void Game::SetRunning(bool R)
{
	running = R;
}

bool Game::IsRunning()
{
	return running;
}
