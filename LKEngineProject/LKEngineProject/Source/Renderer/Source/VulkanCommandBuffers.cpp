#include "../Header/VulkanCommandBuffers.h"

#include "../Header/VulkanSwapchain.h"
#include "../Header/VulkanCommandPool.h"
#include "../Header/VulkanRenderPass.h"
#include "../Header/VulkanPipeline.h"

USING_LK_VULKAN_SPACE

VulkanCommandBuffers::VulkanCommandBuffers(VulkanDevice * device, VkCommandPool& commandPool)
	:VulkanDeviceChild(device),
	commandPool(commandPool)
{
}

void VulkanCommandBuffers::AllocBuffers(size_t size)
{
	commandBuffers.resize(size);

	VkCommandBufferAllocateInfo allocInfo = { };
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	Check_Throw(vkAllocateCommandBuffers(device->GetHandle(), &allocInfo, commandBuffers.data()) != VK_SUCCESS, "Command Buffer 생성 실패");
}

void VulkanCommandBuffers::FreeAll()
{
	vkFreeCommandBuffers(device->GetHandle(), commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}

void VulkanCommandBuffers::RecordBegin(uint32_t index, VkCommandBufferUsageFlags flags)
{
	VkCommandBufferBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.flags = flags;

	vkBeginCommandBuffer(commandBuffers[index], &info);
}

void VulkanCommandBuffers::RecordEnd(uint32_t index)
{
	Check_Throw(vkEndCommandBuffer(commandBuffers[index]) != VK_SUCCESS, "명령 버퍼 기록 실패");
}

const VkCommandBuffer & VulkanCommandBuffers::GetBuffer(uint32_t index) const
{
	return commandBuffers[index];
}

size_t VulkanCommandBuffers::GetBufferSize() const
{
	return commandBuffers.size();
}
