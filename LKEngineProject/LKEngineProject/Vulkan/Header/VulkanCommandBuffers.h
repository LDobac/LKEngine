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

		void AllocBuffers(size_t size);
		void FreeAll();

		void RecordBegin(uint32_t index, VkCommandBufferUsageFlags flags);
		void RecordEnd(uint32_t index);

		const VkCommandBuffer& GetBuffer(uint32_t index) const;
		size_t GetBufferSize() const;
	};
}