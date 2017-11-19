#pragma once

#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanCommandBuffers;

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

		void AllocBuffers(size_t size);
		void FreeBuffers();

		void RecordBegin(uint32_t index, VkCommandBufferUsageFlags flags);
		void RecordEnd(uint32_t index);

		const VkCommandBuffer& GetBuffer(uint32_t index) const;
		size_t GetBufferSize() const;
		const VkCommandPool& GetHandle() const;
	};
}