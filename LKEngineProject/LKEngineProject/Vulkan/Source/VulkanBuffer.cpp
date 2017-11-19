#include "../Header/VulkanBuffer.h"

#include "../Header/VulkanQueue.h"
#include "../Header/VulkanCommandBuffers.h"
#include "../../Utility/Header/Macro.h"

using namespace LKEngine::Vulkan;

VulkanBuffer::VulkanBuffer(VulkanDevice * device)
	:VulkanDeviceChild(device)
{
}

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

VulkanGraphicsBuffer::VulkanGraphicsBuffer(VulkanDevice * device, VulkanCommandPool * commandPool, VulkanQueue * transferQueue)
	:VulkanBuffer(device),
	stagingBuffer(device),
	commandPool(commandPool),
	transferQueue(transferQueue)
{
}

VulkanGraphicsBuffer::~VulkanGraphicsBuffer()
{
}

void VulkanGraphicsBuffer::Init(VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode)
{
	VulkanBuffer::Init(size,
		usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_SHARING_MODE_EXCLUSIVE);

	stagingBuffer.Init(size, 
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE);
}

void VulkanGraphicsBuffer::Shutdown()
{
	VulkanBuffer::Shutdown();
	stagingBuffer.Shutdown();
}

void VulkanGraphicsBuffer::TransferData()
{
	//TODO : 나중에 임시 명령 버퍼 전용 명령 풀 만들기
	//임시 명령풀은 VK_COMMAND_POOL_CREATE_TRANSIMENT_BIT 사용해야 함
	VulkanCommandBuffers cmdBuffers(device,commandPool);
	cmdBuffers.AllocBuffers(1);

	VkBufferCopy copyRegion = {};
	copyRegion.size = bufferSize;

	cmdBuffers.RecordBegin(0, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	vkCmdCopyBuffer(cmdBuffers.GetBuffer(0), stagingBuffer.GetBuffer(), buffer, 1, &copyRegion);

	cmdBuffers.RecordEnd(0);

	VkSubmitInfo submitInfo = { };
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdBuffers.GetBuffer(0);
	transferQueue->Submit(submitInfo);
	transferQueue->WaitIdle();

	cmdBuffers.FreeAll();
}
