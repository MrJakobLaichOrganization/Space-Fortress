#ifndef __LOGGINGMANAGER__
#define __LOGGINGMANAGER__

#include <string>

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
