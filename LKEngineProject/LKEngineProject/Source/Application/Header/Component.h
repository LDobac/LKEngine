#pragma once

#include "../../Utility/Header/Macro.h"

LK_SPACE_BEGIN

class Component
{
public:
	virtual void Start() { }
	virtual void Update() { }
};

LK_SPACE_END