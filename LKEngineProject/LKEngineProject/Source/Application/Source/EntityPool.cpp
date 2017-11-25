#include "../Header/EntityPool.h"

#include "../Header/Entity.h"

#include "../../Renderer/Header/VulkanDescriptorPool.h"
#include "../../Renderer/Header/VulkanDescriptorSetLayout.h"

USING_LK_SPACE

EntityPool::EntityPool()
	:needRender(false)
{
	descriptorPool = new Vulkan::VulkanDescriptorPool();

	{
		Vulkan::VulkanDescriptorSetLayout* cameraLayout = new Vulkan::VulkanDescriptorSetLayout();
		Vulkan::VulkanDescriptorSetLayout* meshLayout = new Vulkan::VulkanDescriptorSetLayout();
		Vulkan::VulkanDescriptorSetLayout* materialLayout = new Vulkan::VulkanDescriptorSetLayout();

		cameraLayout->AddDescriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
		cameraLayout->CreateDescriptorSetLayout();

		meshLayout->AddDescriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
		meshLayout->CreateDescriptorSetLayout();

		materialLayout->AddDescriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0);
		materialLayout->CreateDescriptorSetLayout();

		AddDescriptorSetLayout("DefaultCamera", cameraLayout);
		AddDescriptorSetLayout("DefaultMesh", meshLayout);
		AddDescriptorSetLayout("DefaultMaterial", materialLayout);
	}

	descriptorPool->AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1024);
	descriptorPool->AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024);
	descriptorPool->CreatePool(2048);
}

EntityPool::~EntityPool()
{
	for (auto descriptorSetLayout : descriptorSetLayouts)
	{
		SAFE_DELETE(descriptorSetLayout.second);
	}
	SAFE_DELETE(descriptorPool);

	ReleaseAll();
}

void EntityPool::AddEntity(Entity * newEntity)
{
	entities.push_back(newEntity);
	needRender = true;
}

void EntityPool::RemoveEntity(Entity * removedEntity)
{
	removedEntity->isRemoved = true;
	needRender = true;
}

void EntityPool::Update()
{
	for (auto it = entities.begin(); it != entities.end(); it++)
	{
		if ((*it)->isRemoved)
		{
			SAFE_DELETE(*it);
			it = entities.erase(it);
		}
		else
		{
			(*it)->Update();
		}
	}
}

void EntityPool::Render(const VkCommandBuffer& cmdBuffer)
{
	for (auto entity : entities)
	{
		entity->Render(cmdBuffer);
	}

	needRender = false;
}

void EntityPool::AddDescriptorSetLayout(const std::string& key, Vulkan::VulkanDescriptorSetLayout* setLayout)
{
	descriptorSetLayouts.insert(std::make_pair(key, setLayout));
}

Vulkan::VulkanDescriptorSetLayout * EntityPool::GetDescriptorSetLayout(const std::string& key) const
{
	return descriptorSetLayouts.at(key);
}

Vulkan::VulkanDescriptorPool * EntityPool::GetDescriptorPool() const
{
	return descriptorPool;
}

void EntityPool::ReleaseAll()
{
	for (Entity* entity : entities)
	{
		SAFE_DELETE(entity);
	}
	entities.clear();
}

bool EntityPool::NeedRender() const
{
	return needRender;
}
