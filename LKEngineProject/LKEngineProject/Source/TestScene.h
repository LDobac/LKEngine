#pragma once

#include "../Src/Scene.h"
#include "../Src/Mesh.h"
#include "../Src/Camera.h"

#include "../Vulkan/Header/VulkanMaterial.h"
#include "../Vulkan/Header/VulkanTexture.h"

class TestScene
	: public LKEngine::Scene
{
public:
	virtual void Start() override
	{
		mainCamera = new LKEngine::Camera();
		mainCamera->SetName("Main Camera");
		LKEngine::EntityPool::GetInstance()->AddEntity(mainCamera);

		LKEngine::Vulkan::VulkanMaterial* material = new LKEngine::Vulkan::VulkanMaterial(
			LKEngine::EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultMaterial"),
			LKEngine::PipelineManager::GetInstance()->GetPipeline("Default"));
		material->AddTexture(new LKEngine::Vulkan::VulkanTexture("Textures/chalet.jpg"));
		LKEngine::Mesh* mesh = new LKEngine::Mesh("Models/chalet.obj", material);
		mesh->SetName("Mesh");
		LKEngine::EntityPool::GetInstance()->AddEntity(mesh);

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