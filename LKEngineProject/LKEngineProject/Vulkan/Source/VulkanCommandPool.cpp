#include "../Header/VulkanCommandPool.h"

#include "../Header/VulkanQueue.h"
#include "../Header/VulkanCommandBuffers.h"

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanCommandPool::VulkanCommandPool(VulkanDevice * device)
	: VulkanDeviceChild(device)
{ }

VulkanCommandPool::~VulkanCommandPool()
{
	SAFE_DELETE(commandBuffers);
}

void VulkanCommandPool::Init(VkCommandPoolCreateFlags flags, int32_t queueIndex)
{
	VkCommandPoolCreateInfo poolInfo = { };
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueIndex;
	poolInfo.flags = flags;

	Check_Throw(vkCreateCommandPool(device->GetHandle(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS, "CommandPool 이 생성되지 않음");
}

void VulkanCommandPool::Shutdown()
{
	if (commandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(device->GetHandle(), commandPool, nullptr);
	}
}

void VulkanCommandPool::AllocBuffers(size_t size)
{
	commandBuffers = new VulkanCommandBuffers(device, commandPool);
	commandBuffers->AllocBuffers(size);
}

void VulkanCommandPool::FreeBuffers()
{
	commandBuffers->FreeAll();
	SAFE_DELETE(commandBuffers);
}

void VulkanCommandPool::RecordBegin(uint32_t index, VkCommandBufferUsageFlags flags)
{
	commandBuffers->RecordBegin(index, flags);
}

void VulkanCommandPool::RecordEnd(uint32_t index)
{
	commandBuffers->RecordEnd(index);
}

const VkCommandBuffer & VulkanCommandPool::GetBuffer(uint32_t index) const
{
	return commandBuffers->GetBuffer(index);
}

size_t VulkanCommandPool::GetBufferSize() const
{
	return commandBuffers->GetBufferSize();
}

const VkCommandPool & VulkanCommandPool::GetHandle() const
{
	return commandPool;
}

VulkanSingleCommandPool::VulkanSingleCommandPool(VulkanDevice * device)
	:VulkanDeviceChild(device)
{ }

VulkanSingleCommandPool::~VulkanSingleCommandPool()
{
	SAFE_DELETE(commandBuffers);
}

void VulkanSingleCommandPool::Init(VulkanQueue * queue)
{
	transferQueue = queue;

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queue->GetFamilyIndex();
	poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

	Check_Throw(vkCreateCommandPool(device->GetHandle(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS, "CommandPool 이 생성되지 않음");

	commandBuffers = new VulkanCommandBuffers(device, commandPool);
}

void VulkanSingleCommandPool::Shutdown()
{
	if (commandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(device->GetHandle(), commandPool, nullptr);
	}
}

const VkCommandBuffer & VulkanSingleCommandPool::RecordBegin()
{
	commandBuffers->AllocBuffers(1);

	commandBuffers->RecordBegin(0, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	return commandBuffers->GetBuffer(0);
}

void VulkanSingleCommandPool::RecordEnd()
{
	commandBuffers->RecordEnd(0);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers->GetBuffer(0);

	vkQueueSubmit(transferQueue->GetHandle(), 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(transferQueue->GetHandle());

	commandBuffers->FreeAll();
}

const VkCommandPool & VulkanSingleCommandPool::GetHandle() const
{
	return commandPool;
}