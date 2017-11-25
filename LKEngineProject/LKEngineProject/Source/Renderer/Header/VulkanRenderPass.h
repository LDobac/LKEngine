#pragma once

#include <vulkan/vulkan.h>

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanSwapchain;

class VulkanRenderPass
	: public VulkanDeviceChild
{
private:
	VkRenderPass renderPass;
public:
	explicit VulkanRenderPass(VulkanDevice* device);
	virtual ~VulkanRenderPass();

	void Init(VulkanSwapchain* swapchain);
	virtual void Shutdown();

	void Begin(std::vector<VkClearValue> clearColors, const VkFramebuffer& frameBuffer, VkExtent2D extent, const VkCommandBuffer& cmdBuffer);
	void End(const VkCommandBuffer& cmdBuffer);

	VkRenderPass GetHandle() const;
};

LK_VULKAN_SPACE_END