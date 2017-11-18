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
		void Init(VulkanSwapchain* swapchain, VkCommandPoolCreateFlags flags,int32_t queueIndex);
		virtual void Shutdown() override;

		const VkCommandPool& GetHandle() const;
	};
}