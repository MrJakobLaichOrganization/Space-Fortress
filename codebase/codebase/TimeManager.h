#ifndef __TIMEMANAGER__
#define __TIMEMANAGER__

#include <chrono>

class TimeManager
{
public:
	TimeManager();
	~TimeManager();

	int Init();
	int Exit();

	void Events();
	void Update();
	void Render();

	std::chrono::time_point<std::chrono::high_resolution_clock> getCurrentTime();

	double getTimeDiffMS(std::chrono::time_point<std::chrono::high_resolution_clock> Start, std::chrono::time_point<std::chrono::high_resolution_clock> End);

private:
	int initState = -1;
	// FPS data
	unsigned int fps = 60;
	bool roughFPS = false;
	unsigned int skipMS = 3;
	std::chrono::time_point<std::chrono::high_resolution_clock> globalStartTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
	float TimeDeltaSinceLast = 0.0f;
	float clipTimeDelta = 1.0f;
};

#endif // !__TIMEMANAGER__
