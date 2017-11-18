#include "../Header/VulkanCommandPool.h"

#include "../../Utility/Header/Macro.h"
#include "../Header/VulkanCommandBuffers.h"

using namespace LKEngine::Vulkan;

VulkanCommandPool::VulkanCommandPool(VulkanDevice * device)
	: VulkanDeviceChild(device)
{
}

VulkanCommandPool::~VulkanCommandPool()
{
	SAFE_DELETE(commandBuffers);
}

void VulkanCommandPool::Init(VkCommandPoolCreateFlags flags, int32_t queueIndex)
{
	Console_Log("명령 풀 생성 시작");
	VkCommandPoolCreateInfo poolInfo = { };
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueIndex;
	poolInfo.flags = flags;

	Check_Throw(vkCreateCommandPool(device->GetHandle(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS, "CommandPool 이 생성되지 않음");
	Console_Log("명령 풀 생성 성공");
}

void VulkanCommandPool::Shutdown()
{
	if (commandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(device->GetHandle(), commandPool, nullptr);
	}
}

void VulkanCommandPool::AllocBuffers(VulkanSwapchain* swapchain)
{
	commandBuffers = new VulkanCommandBuffers(device, this);
	commandBuffers->Init(swapchain);
}

void VulkanCommandPool::FreeBuffers()
{
	commandBuffers->Free();
	SAFE_DELETE(commandBuffers);
}

void VulkanCommandPool::Record(VulkanSwapchain * swapchain, VulkanRenderPass * renderPass, VulkanGraphicsPipeline * graphicsPipeline, std::vector<VkClearValue> clearColor)
{
	commandBuffers->Record(swapchain, renderPass, graphicsPipeline, clearColor);
}

const VkCommandBuffer & VulkanCommandPool::GetBuffer(uint32_t index) const
{
	return commandBuffers->GetBuffer(index);
}

const VkCommandPool & VulkanCommandPool::GetHandle() const
{
	return commandPool;
}
