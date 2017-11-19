#pragma once

#include <vulkan/vulkan.h>

#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanSemaphore;
	class VulkanSwapchain;

	class VulkanQueue
		: VulkanDeviceChild
	{
	private:
		VkQueue vkQueue;

		uint32_t familyIndex;
		uint32_t queueIndex;
	public:
		explicit VulkanQueue(VulkanDevice* device, uint32_t familyIndex, uint32_t queueIndex);

		void Submit(VulkanSemaphore* waitSemaphore, VulkanSemaphore* signalSemaphore,VkPipelineStageFlags* stageFlags ,const VkCommandBuffer& commandBuffer);
		void Submit(const VkSubmitInfo& submitInfo);
		void Present(VulkanSwapchain* swapchain, VulkanSemaphore* waitSemaphore,uint32_t imageIndex);

		void WaitIdle();

		VkQueue GetHandle() const;
		uint32_t GetFamilyIndex() const;
	};
}