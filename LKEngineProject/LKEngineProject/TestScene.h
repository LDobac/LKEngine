#pragma once

#include "Source/LKEngine.h"

#include "Source/Application/Header/MeshLoader.h"

USING_LK_SPACE;

class TestScene
	: public Scene
{
private:
	Mesh* mesh1;
	Mesh* mesh2;
public:
	virtual void Start() override
	{
		mainCamera = new Camera();
		mainCamera->SetName("Main Camera");
		mainCamera->SetPosition(glm::vec3(2.0f, 2.0f, 2.0f));
		EntityPool::GetInstance()->AddEntity(mainCamera);

		Texture* texture = AssetManager::GetInstance()->Load<Texture>("Textures/chalet.jpg");
		Material* material1 = new Material(
			EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultMaterial"),
			PipelineManager::GetInstance()->GetPipeline("Default"));
		material1->AddTexture(texture);
		Material* material2 = new LKEngine::Material(
			EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultMaterial"),
			PipelineManager::GetInstance()->GetPipeline("Default"));
		material2->AddTexture(texture);

		mesh1 = AssetManager::GetInstance()->Load<Mesh>("Models/chalet.obj");
		mesh1->AddMaterial(material1);
		mesh1->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
		mesh1->SetName("Mesh1");
		EntityPool::GetInstance()->AddEntity(mesh1);

		mesh2 = AssetManager::GetInstance()->Load<Mesh>("Models/chalet.obj");
		mesh2->AddMaterial(material2);
		mesh2->SetName("Mesh2");
		EntityPool::GetInstance()->AddEntity(mesh2);

		Console_Log("Test Scene Start");
	}
	virtual void Update() override
	{
		mesh1->SetRotation(glm::vec3(0.3f, 0.1f, 0.2f) * LKEngine::Time::GetTime());
		mesh2->SetRotation(glm::vec3(0.5f, 0.2f, 0.4f) * LKEngine::Time::GetTime());
	}
	virtual void End() override
	{
		Console_Log("Test Scene End");
	}
};