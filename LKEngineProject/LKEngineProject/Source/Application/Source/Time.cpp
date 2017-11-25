#include "../Header/Time.h"

USING_LK_SPACE

float Time::time = 0.0f;
float Time::elapsedTime = 0.0f;
std::chrono::high_resolution_clock::time_point Time::preTime = std::chrono::high_resolution_clock::now();

void Time::Start()
{
	preTime = std::chrono::high_resolution_clock::now();
}

void Time::Update()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - preTime).count() * 0.001f;
	time += elapsedTime;

	preTime = currentTime;
}

float Time::GetTime()
{
	return time;
}

float Time::GetElapsedTime()
{
	return elapsedTime;
}

