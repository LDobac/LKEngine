#pragma once

#include "Source/LKEngine.h"

#include "Source/Application/Header/MeshLoader.h"

USING_LK_SPACE;

class TestScene
	: public Scene
{
private:
	Mesh* mesh;
public:
	virtual void Start() override
	{
		mainCamera = new Camera();
		mainCamera->SetName("Main Camera");
		mainCamera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
		EntityPool::GetInstance()->AddEntity(mainCamera);

		Texture* texture = AssetManager::GetInstance()->Load<Texture>("Textures/chalet.jpg");
		Material* material = new Material(
			EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultMaterial"),
			PipelineManager::GetInstance()->GetPipeline("Default"));
		material->AddTexture(texture);

		mesh = AssetManager::GetInstance()->Load<Mesh>("Models/chalet.obj");
		mesh->AddMaterial(material);
		mesh->SetPosition(glm::vec3(0.0f, 0.0f, -2.0f));
		mesh->SetName("Mesh");
		EntityPool::GetInstance()->AddEntity(mesh);

		Console_Log("Test Scene Start");
	}
	virtual void Update() override
	{
	}
	virtual void End() override
	{
		Console_Log("Test Scene End");
	}
};