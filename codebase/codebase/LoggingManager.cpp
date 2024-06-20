#include "LoggingManager.h"

#include <iostream>

LoggingManager::LoggingManager()
{
}

LoggingManager::~LoggingManager()
{
}

int LoggingManager::Init()
{
	initState = 0;

	return initState;
}

int LoggingManager::Exit()
{
	initState;

	return initState;
}

void LoggingManager::Event()
{
}

void LoggingManager::Update()
{
}

void LoggingManager::Render()
{
}

void LoggingManager::log(std::string text)
{
	// this sucks ASS for performance, console is also disabled for release btw, this will therefore be expanded into a thread that pushes to file.
	std::cout << text << "\n";
}
