#ifndef __GAME__
#define __GAME__

/*
* main game file, this is static, this stores all the relevant data.
*/

#include "LoggingManager.h"
#include "GraphicsManager.h"
#include "EventsManager.h"

class Game
{
public:
	~Game() {}

	static Game& S()
	{
		static Game game;
		return game;
	}

	int Init();
	int Exit();

	int Loop();

	void SetRunning(bool R);
	bool IsRunning();

	LoggingManager LOGGER;
	GraphicsManager GRAPHICS;
	EventsManager EVENTS;

private:
	Game() {}

	int initState = -1;
	bool running = false;
};

#endif // !__GAME__
