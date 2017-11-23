#include "SceneManager.h"

#include "Scene.h"

USING_LK_SPACE

SceneManager::~SceneManager()
{
	if(newScene) SAFE_DELETE(newScene);

	if (currentScene)
	{
		currentScene->End();
		SAFE_DELETE(currentScene);
	}
}

void SceneManager::Start(Scene * scene)
{
	newScene = nullptr;
	currentScene = scene;

	scene->Start();
}

void SceneManager::Load(Scene * scene)
{
	newScene = scene;
}

void SceneManager::Update()
{
	if (newScene)
	{
		currentScene->End();
		newScene->Start();

		SAFE_DELETE(currentScene);

		currentScene = newScene;
		newScene = nullptr;
	}

	currentScene->Update();
}

Scene * SceneManager::GetCurrentScene() const
{
	return currentScene;
}
