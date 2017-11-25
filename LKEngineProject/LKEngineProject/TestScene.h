#pragma once

#include "Source/Renderer/Header/VulkanMaterial.h"
#include "Source/Renderer/Header/VulkanTexture.h"

#include "Source/Application/Header/Scene.h"
#include "Source/Application/Header/Mesh.h"
#include "Source/Application/Header/Camera.h"

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
		material->AddTexture(new LKEngine::Vulkan::VulkanTexture("Assets/Textures/chalet.jpg"));
		LKEngine::Mesh* mesh = new LKEngine::Mesh("Assets/Models/chalet.obj", material);
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