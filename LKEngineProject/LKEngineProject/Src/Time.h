#pragma once

#include <chrono>

#include "../Utility/Header/Macro.h"

LK_SPACE_BEGIN

class Time
{
private:
	static std::chrono::high_resolution_clock::time_point preTime;

	static float time;
	static float elapsedTime;
public:
	static void Start();
	
	static void Update();
	
	static float GetTime();
	static float GetElapsedTime();
	
};

LK_SPACE_END