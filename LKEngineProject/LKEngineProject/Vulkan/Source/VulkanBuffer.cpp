#include "../Header/VulkanBuffer.h"

#include "../Header/VulkanQueue.h"
#include "../Header/VulkanCommandPool.h"

USING_LK_VULKAN_SPACE

VulkanBuffer::VulkanBuffer(VulkanDevice * device)
	:VulkanDeviceChild(device)
{ }

void VulkanBuffer::Init(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkSharingMode sharingMode)
{
	VkBufferCreateInfo info = { };
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.size = size;
	info.usage = usage;
	info.sharingMode = sharingMode;
	bufferSize = size;

	Check_Throw(vkCreateBuffer(device->GetHandle(), &info, nullptr, &buffer) != VK_SUCCESS, "버퍼 생성 실패");

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device->GetHandle(), buffer, &memRequirements);
	
	VkMemoryAllocateInfo allocInfo = { };
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, properties);
	Check_Throw(vkAllocateMemory(device->GetHandle(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS, "버퍼 메모리 할당 실패");

	vkBindBufferMemory(device->GetHandle(), buffer, bufferMemory, 0);
}

void VulkanBuffer::Shutdown()
{
	vkDestroyBuffer(device->GetHandle(), buffer, nullptr);
	vkFreeMemory(device->GetHandle(), bufferMemory, nullptr);
}

const VkBuffer & VulkanBuffer::GetBuffer() const
{
	return buffer;
}

const VkDeviceMemory & VulkanBuffer::GetBufferMemory() const
{
	return bufferMemory;
}

const VkDeviceSize VulkanBuffer::GetBufferSize() const
{
	return bufferSize;
}

void VulkanBuffer::CopyBuffer(VulkanBuffer * dstBuffer, VulkanSingleCommandPool * commandPool)
{
	VkBufferCopy copyRegion = {};
	copyRegion.size = bufferSize;

	VkCommandBuffer cmdBuffer = commandPool->RecordBegin();

	vkCmdCopyBuffer(cmdBuffer, buffer, dstBuffer->GetBuffer(), 1, &copyRegion);

	commandPool->RecordEnd();
}
