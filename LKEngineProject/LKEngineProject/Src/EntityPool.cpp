#include "EntityPool.h"

#include "Entity.h"

USING_LK_SPACE

EntityPool::EntityPool()
{
}

EntityPool::~EntityPool()
{
	ReleaseAll();
}

void EntityPool::AddEntity(Entity * newEntity)
{
	entities.push_back(newEntity);
}

void EntityPool::Update()
{
	for (auto it = entities.begin(); it != entities.end(); it++)
	{
		if ((*it)->isRemoved)
		{
			SAFE_DELETE(*it);
			it = entities.erase(it);
		}
		else
		{
			(*it)->Update();
		}
	}
}

void EntityPool::ReleaseAll()
{
	for (Entity* entity : entities)
	{
		SAFE_DELETE(entity);
	}
	entities.clear();
}
