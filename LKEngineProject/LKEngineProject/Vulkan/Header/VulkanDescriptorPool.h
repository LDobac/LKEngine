#pragma once

#include "../Header/VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanDescriptorPool
		: public VulkanDeviceChild
	{
	private:
		VkDescriptorPool descriptorPool;

		std::vector<VkDescriptorPoolSize> poolSizes;
	public:
		explicit VulkanDescriptorPool(VulkanDevice* device);

		void AddPoolSize(VkDescriptorType type,size_t descriptorCount);
		void CreatePool(size_t maxSets = 1);

		void Shutdown();

		const VkDescriptorPool& GetHandle() const;
	};
}