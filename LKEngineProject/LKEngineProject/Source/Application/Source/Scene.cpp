#include "../Header/Scene.h"

#include "../Header/EntityPool.h"

USING_LK_SPACE

Scene::Scene()
{
	entityPool = new EntityPool();
}

Scene::~Scene()
{
	SAFE_DELETE(entityPool);
}

EntityPool * Scene::GetPool() const
{
	return entityPool;
}
