#pragma once

#include "../Src/Scene.h"

class TestScene
	: public LKEngine::Scene
{
public:
	virtual void Start() override
	{
		Console_Log("Test Scene Start");
	}
	virtual void Update() override
	{
		Console_Log("Test Scene Update");
	}
	virtual void End() override
	{
		Console_Log("Test Scene End");
	}
};