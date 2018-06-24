#pragma once

#include <vector>

#include "Object.h"

#include "../../Utility/Header/TSingleton.h"

LK_SPACE_BEGIN

class Entity;

class EntityPool : public Object
{
private:
	std::vector<Entity*> entities;

public:
	explicit EntityPool();
	virtual ~EntityPool();

	void Update();

	void AddEntity(const Entity* newEntity);

	virtual void Update() override;

	void RemoveEntity(const Entity*  removedEntity);
	void RemoveAll();
};

LK_SPACE_END