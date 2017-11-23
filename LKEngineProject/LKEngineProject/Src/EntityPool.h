#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

#include "../Utility/Header/TSingleton.h"

LK_SPACE_BEGIN

namespace Vulkan
{
	class VulkanDescriptorSetLayout;
	class VulkanDescriptorPool;
}

class Entity;

class EntityPool
	: public TSingleton<EntityPool>
{
private:
	bool needRender;

	std::vector<Entity*> entities;

	Vulkan::VulkanDescriptorSetLayout* descriptorSetLayout;
	Vulkan::VulkanDescriptorPool* descriptorPool;
public:
	explicit EntityPool();
	virtual ~EntityPool();

	void AddEntity(Entity* newEntity);

	void RemoveEntity(Entity* removedEntity);

	void Update();
	void Render(const VkCommandBuffer& cmdBuffer);

	Vulkan::VulkanDescriptorSetLayout* GetDescriptorSetLayout() const;
	Vulkan::VulkanDescriptorPool* GetDescriptorPool() const;

	void ReleaseAll();

	bool NeedRender() const;
};

LK_SPACE_END