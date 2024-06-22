#ifndef __LOGGINGMANAGER__
#define __LOGGINGMANAGER__

#include <string>

// Note that the global logger exists in the `Game` class
// Game::S().LOGGER.log is the function to use.

class LoggingManager
{
public:
	LoggingManager();
	~LoggingManager();

	int Init();
	int Exit();

	void Event();
	void Update();
	void Render();

	void log(std::string text);

private:
	int initState = -1;
};

#endif // !__LOGGINGMANAGER__
