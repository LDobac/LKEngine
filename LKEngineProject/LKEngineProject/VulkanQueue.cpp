#include "VulkanQueue.h"

#include "VulkanDevice.h"

LKEngine::Vulkan::VulkanQueue::VulkanQueue(VulkanDevice * device, uint32_t familyIndex, uint32_t queueIndex)
	:device(device)
{
	vkGetDeviceQueue(device->GetRawDevice(), familyIndex, queueIndex, &vkQueue);
}
