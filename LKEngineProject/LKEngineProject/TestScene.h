#pragma once

#include "Source/LKEngine.h"

#include "Source/Application/Header/MeshLoader.h"
#include "Source/Application/Header/EntityPool.h"

#include "Source/Application/Header/Entity.h"
#include "Source/Application/Header/RendererComponent.h"

USING_LK_SPACE;

class TestScene
	: public Scene
{
private:
	Camera * camera;

	Entity* mesh;
public:
	virtual void Begin() override
	{
		camera = new Camera();
		camera->SetName("Main Camera");
		camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
		GetPool()->AddEntity(camera);

		Texture* texture = AssetManager::GetInstance()->Load<Texture>("Textures/chalet.jpg");
		Material* material = new Material(PipelineManager::GetInstance()->GetPipeline("Default"));
		material->AddTexture(texture);

		mesh = new Entity();
		mesh->SetPosition(glm::vec3(0.0f, 0.0f, -2.0f));
		mesh->SetName("Mesh");
		GetPool()->AddEntity(mesh);

		RendererComponent* renderer = new RendererComponent(mesh, AssetManager::GetInstance()->Load<Mesh>("Models/chalet.obj"));
		renderer->AddMaterial(material);

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