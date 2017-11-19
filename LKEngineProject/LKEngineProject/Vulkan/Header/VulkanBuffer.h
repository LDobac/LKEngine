#pragma once

#include "../../Utility/Header/Macro.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanSingleCommandPool;
	class VulkanQueue;

	class VulkanBuffer
		: public VulkanDeviceChild
	{
	protected:
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;

		VkDeviceSize bufferSize;
	public:
		explicit VulkanBuffer(VulkanDevice* device);

		void Init(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties ,VkSharingMode sharingMode);
		virtual void Shutdown();

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

		void CopyBuffer(VulkanBuffer* dstBuffer, VulkanSingleCommandPool* commandPool);
		template<typename T>
		void CopyLocalMemory(const T* data, VulkanSingleCommandPool* commandPool)
		{
			VulkanBuffer* stagingBuffer = new VulkanBuffer(device);
			stagingBuffer->Init(
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				VK_SHARING_MODE_EXCLUSIVE);
			stagingBuffer->Map(data);
			stagingBuffer->CopyBuffer(this, commandPool);
			stagingBuffer->Shutdown();
			SAFE_DELETE(stagingBuffer);
		}
	};
}