#pragma once

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanSwapchain;
class VulkanRenderPass;
class VulkanGraphicsPipeline;

class VulkanCommandBuffers
	: public VulkanDeviceChild
{
private:
	VkCommandPool& commandPool;

	std::vector<VkCommandBuffer> commandBuffers;
public:
	explicit VulkanCommandBuffers(VulkanDevice* device, VkCommandPool& commandPool);

	void AllocBuffers(size_t size);
	void FreeAll();

	void RecordBegin(uint32_t index, VkCommandBufferUsageFlags flags);
	void RecordEnd(uint32_t index);

	const VkCommandBuffer& GetBuffer(uint32_t index) const;
	size_t GetBufferSize() const;
};

LK_VULKAN_SPACE_END