#pragma once

#include "EntityPool.h"

#include "../../Utility/Header/Macro.h"

LK_SPACE_BEGIN

class Camera;

class Scene
{
protected:
	Camera* mainCamera;
public:
	explicit Scene();
	virtual ~Scene();

	virtual void Start();
	virtual void Update();
	virtual void End();

	Camera* GetMainCamera() const;
};

LK_SPACE_END