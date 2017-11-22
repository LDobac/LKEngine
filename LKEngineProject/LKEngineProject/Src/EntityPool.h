#pragma once

#include <vector>

#include "../Utility/Header/Macro.h"

LK_SPACE_BEGIN

#include "../Utility/Header/TSingleton.h"

class Entity;

class EntityPool
	: public TSingleton<EntityPool>
{
private:
	std::vector<Entity*> entities;
public:
	explicit EntityPool();
	virtual ~EntityPool();

	void AddEntity(Entity* newEntity);

	void Update();

	void ReleaseAll();
};

LK_SPACE_END