#include "TimeManager.h"

#include <SDL.h>

#include "Game.h"

TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}

int TimeManager::Init()
{
	initState = 0;

	if (initState == 0)
	{
		fps = 60;
		roughFPS = false;
		skipMS = 3;

		startTimePoint = std::chrono::high_resolution_clock::now();
		globalStartTime = std::chrono::high_resolution_clock::now();
	}

	return initState;
}

int TimeManager::Exit()
{
	initState;

	return initState;
}

void TimeManager::Events()
{
}

void TimeManager::Update()
{
	auto nowTime = getCurrentTime();
	std::chrono::duration<double> diff = nowTime - startTimePoint;
	std::chrono::microseconds high_res_time_diff = std::chrono::microseconds(1000 * 1000 / fps);
	std::chrono::milliseconds millis_timer = std::chrono::milliseconds(skipMS);
	while (diff < high_res_time_diff)
	{
		if (roughFPS)
		{
			if (diff > millis_timer)
			{
				SDL_Delay(1);
			}
		}
		auto nowTime = getCurrentTime();
		diff = nowTime - startTimePoint;
	}
	//startTimePoint = nowTime;
	std::chrono::milliseconds low_res_time_diff = std::chrono::milliseconds(1000 / fps + 1);
	if (diff > low_res_time_diff)
	{
		Game::S().LOGGER.log("frame took too long, took " + std::to_string(diff.count()) + " s" +
			" (" + std::to_string(diff.count() * 1000) + "ms)");
	}
	TimeDeltaSinceLast = diff.count();
	//FPSdata.push(TimeDeltaSinceLast);
	if (TimeDeltaSinceLast > clipTimeDelta)
	{
		Game::S().LOGGER.log("TimeDelta was " + std::to_string(TimeDeltaSinceLast) + ", clipping TimeDelta to " + std::to_string(clipTimeDelta));
		TimeDeltaSinceLast = clipTimeDelta;
	}
	startTimePoint = getCurrentTime();
}

void TimeManager::Render()
{
}

std::chrono::time_point<std::chrono::high_resolution_clock> TimeManager::getCurrentTime()
{
	return std::chrono::high_resolution_clock::now();
}

double TimeManager::getTimeDiffMS(std::chrono::time_point<std::chrono::high_resolution_clock> Start, std::chrono::time_point<std::chrono::high_resolution_clock> End)
{
	std::chrono::duration<double> diff = End - Start;
	return diff.count();
}
