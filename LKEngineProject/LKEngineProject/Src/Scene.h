#pragma once

#include "EntityPool.h"

#include "../Utility/Header/Macro.h"

LK_SPACE_BEGIN

class Scene
{
protected:

public:
	explicit Scene();
	virtual ~Scene();

	virtual void Start();
	virtual void Update();
	virtual void End();
};

LK_SPACE_END