#ifndef __GAME__
#define __GAME__

/*
* main game file, this is static, this stores all the relevant data.
*/

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
private:
	Game() {}

	int initState = -1;
	int exitState = -1;
	bool running = false;
};

#endif // !__GAME__
