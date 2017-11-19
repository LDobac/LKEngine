#pragma once

#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanDescriptorSet
		: public VulkanDeviceChild
	{
	private:
		VkDescriptorSet descriptorSet;

		std::vector<VkDescriptorBufferInfo> bufferInfos;
		std::vector<VkWriteDescriptorSet> descriptorWrites;
	public:
		explicit VulkanDescriptorSet(VulkanDevice* device);

		void Init(VulkanDescriptorSetLayout* setLayout, VulkanDescriptorPool* pool);

		void AddBufferInfo(VkDescriptorType type, VulkanBuffer* buffer, size_t offset, uint32_t binding);

		void UpdateSets();

		const VkDescriptorSet& GetHandle() const;
	};
}