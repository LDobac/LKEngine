#pragma once

#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanSwapchain;
	class VulkanCommandPool;
	class VulkanRenderPass;
	class VulkanGraphicsPipeline;

	class VulkanCommandBuffers
		: public VulkanDeviceChild
	{
	private:
		VulkanCommandPool* commandPool;

		std::vector<VkCommandBuffer> commandBuffers;
	public:
		explicit VulkanCommandBuffers(VulkanDevice* device, VulkanCommandPool* commandPool);

		void Init(VulkanSwapchain* swapchain);
		void Free();

		void Record(VulkanSwapchain* swapchain, VulkanRenderPass* renderPass, VulkanGraphicsPipeline* graphicsPipeline, std::vector<VkClearValue> clearColor);

		const VkCommandBuffer& GetBuffer(uint32_t index) const;
	};
}