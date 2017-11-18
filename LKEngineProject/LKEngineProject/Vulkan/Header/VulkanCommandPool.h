#pragma once

#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanCommandBuffers;
	class VulkanSwapchain;

	class VulkanCommandPool
		: public IVulkanObject, public VulkanDeviceChild
	{
	private:
		VkCommandPool commandPool = VK_NULL_HANDLE;

		VulkanCommandBuffers* commandBuffers;
	public:
		VulkanCommandPool(VulkanDevice* device);
		~VulkanCommandPool();

		virtual void Init() override {}
		void Init(VkCommandPoolCreateFlags flags,int32_t queueIndex);
		virtual void Shutdown() override;

		void AllocBuffers(VulkanSwapchain* swapchain);
		void FreeBuffers();

		void Record(VulkanSwapchain * swapchain, VulkanRenderPass * renderPass, VulkanGraphicsPipeline * graphicsPipeline, std::vector<VkClearValue> clearColor);

		const VkCommandBuffer& GetBuffer(uint32_t index) const;
		const VkCommandPool& GetHandle() const;
	};
}