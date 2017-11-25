#pragma once

#include "Source/LKEngine.h"

class TestScene
	: public LKEngine::Scene
{
private:
	LKEngine::Mesh* mesh1;
	LKEngine::Mesh* mesh2;
public:
	virtual void Start() override
	{
		mainCamera = new LKEngine::Camera();
		mainCamera->SetName("Main Camera");
		mainCamera->SetPosition(glm::vec3(2.0f, 2.0f, 2.0f));
		LKEngine::EntityPool::GetInstance()->AddEntity(mainCamera);

		LKEngine::Vulkan::VulkanMaterial* material1 = new LKEngine::Vulkan::VulkanMaterial(
			LKEngine::EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultMaterial"),
			LKEngine::PipelineManager::GetInstance()->GetPipeline("Default"));
		material1->AddTexture(new LKEngine::Vulkan::VulkanTexture("Assets/Textures/chalet.jpg"));
		mesh1 = new LKEngine::Mesh("Assets/Models/chalet.obj", material1);
		mesh1->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
		mesh1->SetScale(glm::vec3(0.3f, 0.7f, 1.2f));
		mesh1->SetName("Mesh1");
		LKEngine::EntityPool::GetInstance()->AddEntity(mesh1);

		LKEngine::Vulkan::VulkanMaterial* material2 = new LKEngine::Vulkan::VulkanMaterial(
			LKEngine::EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultMaterial"),
			LKEngine::PipelineManager::GetInstance()->GetPipeline("Default"));
		material2->AddTexture(new LKEngine::Vulkan::VulkanTexture("Assets/Textures/chalet.jpg"));
		mesh2 = new LKEngine::Mesh("Assets/Models/chalet.obj", material2);
		mesh2->SetName("Mesh2");
		mesh2->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
		LKEngine::EntityPool::GetInstance()->AddEntity(mesh2);

		Console_Log("Test Scene Start");
	}
	virtual void Update() override
	{
		mesh1->SetRotation(glm::vec3(0.3f, 0.1f, 0.2f) * LKEngine::Time::GetTime());
		mesh2->SetRotation(glm::vec3(0.5f, 0.2f, 0.4f) * LKEngine::Time::GetTime());
		LKEngine::Application::GetInstance()->Exit();
	}
	virtual void End() override
	{
		Console_Log("Test Scene End");
	}
};