#include "../Header/VulkanQueue.h"

#include "../Header/VulkanDevice.h"

using namespace LKEngine::Vulkan;

VulkanQueue::VulkanQueue(VulkanDevice * device, uint32_t familyIndex, uint32_t queueIndex)
	:VulkanDeviceChild(device)
{
	vkGetDeviceQueue(device->GetHandle(), familyIndex, queueIndex, &vkQueue);
}

uint32_t LKEngine::Vulkan::VulkanQueue::GetFamilyIndex() const
{
	return familyIndex;
}

VkQueue VulkanQueue::GetHandle() const
{
	return vkQueue;
}
