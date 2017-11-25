#pragma once

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

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
	explicit VulkanBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode);
	virtual ~VulkanBuffer();

	template<typename T>
	inline void Map(const T* data,int offset = 0,VkMemoryMapFlags flags = 0)
	{
		T* mapped;
		vkMapMemory(device->GetHandle(), bufferMemory, offset, bufferSize, flags, reinterpret_cast<void**>(&mapped));
		std::memcpy(mapped, data, size_t(bufferSize));
		vkUnmapMemory(device->GetHandle(), bufferMemory);
	}

	void CopyBuffer(VulkanBuffer* dstBuffer);
	template<typename T>
	inline void CopyDataLocalMemory(const T* data)
	{
		VulkanBuffer* stagingBuffer = new VulkanBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			VK_SHARING_MODE_EXCLUSIVE
		);
		stagingBuffer->Map(data);
		stagingBuffer->CopyBuffer(this);
		SAFE_DELETE(stagingBuffer);
	}

	const VkBuffer& GetBuffer() const;
	const VkDeviceMemory& GetBufferMemory() const;
	const VkDeviceSize GetBufferSize() const;
};

LK_VULKAN_SPACE_END