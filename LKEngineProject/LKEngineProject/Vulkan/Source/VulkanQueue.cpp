#include "../Header/VulkanQueue.h"

#include "../Header/VulkanDevice.h"
#include "../Header/VulkanSemaphore.h"
#include "../Header/VulkanSwapchain.h"

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanQueue::VulkanQueue(VulkanDevice * device, uint32_t familyIndex, uint32_t queueIndex)
	:VulkanDeviceChild(device)
{
	vkGetDeviceQueue(device->GetHandle(), familyIndex, queueIndex, &vkQueue);
}

void VulkanQueue::Submit(VulkanSemaphore* waitSemaphore, VulkanSemaphore* signalSemaphore, VkPipelineStageFlags* stageFlags, const VkCommandBuffer& commandBuffer)
{
	VkSubmitInfo submitInfo = { };
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &waitSemaphore->GetHandle();
	submitInfo.pWaitDstStageMask = stageFlags;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &signalSemaphore->GetHandle();

	Check_Throw(vkQueueSubmit(vkQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS, "Queue Submit ½ÇÆÐ");
}

void VulkanQueue::Submit(const VkSubmitInfo & submitInfo)
{
	vkQueueSubmit(vkQueue, 1, &submitInfo, VK_NULL_HANDLE);
}

void VulkanQueue::Present(VulkanSwapchain * swapchain, VulkanSemaphore* waitSemaphore, uint32_t imageIndex)
{
	VkPresentInfoKHR info = { };
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &waitSemaphore->GetHandle();

	info.swapchainCount = 1;
	info.pSwapchains = &swapchain->GetHandle();
	info.pImageIndices = &imageIndex;
	info.pResults = nullptr;

	vkQueuePresentKHR(vkQueue, &info);
}

void VulkanQueue::WaitIdle()
{
	vkQueueWaitIdle(vkQueue);
}

uint32_t LKEngine::Vulkan::VulkanQueue::GetFamilyIndex() const
{
	return familyIndex;
}

VkQueue VulkanQueue::GetHandle() const
{
	return vkQueue;
}
