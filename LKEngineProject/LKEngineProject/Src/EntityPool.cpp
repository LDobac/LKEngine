#include "EntityPool.h"

#include "Entity.h"

#include "../Vulkan/Header/VulkanDescriptorPool.h"
#include "../Vulkan/Header/VulkanDescriptorSetLayout.h"

USING_LK_SPACE

EntityPool::EntityPool()
	:needRender(false)
{
	descriptorSetLayout = new Vulkan::VulkanDescriptorSetLayout();
	descriptorPool = new Vulkan::VulkanDescriptorPool();

	descriptorSetLayout->AddDescriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	descriptorSetLayout->AddDescriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
	descriptorSetLayout->CreateDescriptorSetLayout();

	descriptorPool->AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1);
	descriptorPool->AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1);
	descriptorPool->CreatePool();
}

EntityPool::~EntityPool()
{
	SAFE_DELETE(descriptorSetLayout);
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
}

Vulkan::VulkanDescriptorSetLayout * EntityPool::GetDescriptorSetLayout() const
{
	return descriptorSetLayout;
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
