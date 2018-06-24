#pragma once

#include "Object.h"
#include "Entity.h"

LK_SPACE_BEGIN

class Component : public Object
{
private:
	Entity* entity;

public:
	explicit Component(Entity* entity) : entity(entity) { }

	virtual void Begin() = 0;
	virtual void Update() = 0;
	virtual void End() = 0;

	Entity* GetEntity() const { return entity; }
};

LK_SPACE_END