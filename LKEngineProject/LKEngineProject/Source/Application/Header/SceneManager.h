#pragma once

#include "../../Utility/Header/TSingleton.h"

LK_SPACE_BEGIN

class Scene;

class SceneManager
	: public TSingleton<SceneManager>
{
private:
	Scene* newScene;
	Scene* currentScene;
public:
	virtual ~SceneManager();

	void Start(Scene* scene);

	void Load(Scene* scene);

	void Update();

	Scene* GetCurrentScene() const;
};

LK_SPACE_END