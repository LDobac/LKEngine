#pragma once

#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanCommandPool;
	class VulkanQueue;

	class VulkanBuffer
		: public IVulkanObject, public VulkanDeviceChild
	{
	protected:
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;

		VkDeviceSize bufferSize;
	public:
		explicit VulkanBuffer(VulkanDevice* device);

		virtual void Init() override { }
		void Init(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties ,VkSharingMode sharingMode);
		virtual void Shutdown() override;

		template<typename T>
		inline void Map(const T* data,int offset = 0,VkMemoryMapFlags flags = 0)
		{
			T* mapped;
			vkMapMemory(device->GetHandle(), bufferMemory, offset, bufferSize, flags, reinterpret_cast<void**>(&mapped));
			std::memcpy(mapped, data, size_t(bufferSize));
			vkUnmapMemory(device->GetHandle(), bufferMemory);
		}

		const VkBuffer& GetBuffer() const;
		const VkDeviceMemory& GetBufferMemory() const;
		const VkDeviceSize GetBufferSize() const;

		void CopyBuffer(VulkanBuffer* dstBuffer, VulkanCommandPool* commandPool, VulkanQueue* transferQueue);
	};
}