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
	};

	class VulkanGraphicsBuffer
		: public VulkanBuffer
	{
	private:
		VulkanCommandPool* commandPool;
		VulkanQueue* transferQueue;

		VulkanBuffer stagingBuffer;
	public:
		explicit VulkanGraphicsBuffer(VulkanDevice* device,VulkanCommandPool* commandPool,VulkanQueue* transferQueue);
		~VulkanGraphicsBuffer();

		void Init(VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode);
		virtual void Shutdown() override;

		template<typename T>
		inline void Map(const T* data, int offset = 0, VkMemoryMapFlags flags = 0)
		{
			T* mapped;
			vkMapMemory(device->GetHandle(),stagingBuffer.GetBufferMemory(), offset, bufferSize, flags, reinterpret_cast<void**>(&mapped));
			std::memcpy(mapped, data, size_t(bufferSize));
			vkUnmapMemory(device->GetHandle(), stagingBuffer.GetBufferMemory());

			TransferData();
		}

	private:
		void TransferData();
	};
}