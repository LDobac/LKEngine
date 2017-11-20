#pragma once

#include "../Utility/Header/Macro.h"

LK_SPACE_BEGIN

class Scene
{
public:
	explicit Scene();
	virtual ~Scene();

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void End() = 0;
};

LK_SPACE_END