#pragma once

#include "Object.h"

LK_SPACE_BEGIN

class EntityPool;

class Scene : public Object
{
private:
	EntityPool * entityPool;

protected:
	EntityPool * GetPool() const;

public:
	explicit Scene();
	~Scene();

	virtual void Begin() = 0;
	virtual void Update() = 0;
	virtual void End() = 0;
};

LK_SPACE_END