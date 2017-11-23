#pragma once

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanDescriptorPool
	: public VulkanDeviceChild
{
private:
	VkDescriptorPool descriptorPool;

	std::vector<VkDescriptorPoolSize> poolSizes;
public:
	explicit VulkanDescriptorPool();
	~VulkanDescriptorPool();

	void AddPoolSize(VkDescriptorType type,size_t descriptorCount);
	void CreatePool(size_t maxSets = 1);

	const VkDescriptorPool& GetHandle() const;
};

LK_VULKAN_SPACE_END