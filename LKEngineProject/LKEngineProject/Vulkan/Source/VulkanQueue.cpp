#include "../Header/VulkanQueue.h"

#include "../Header/VulkanDevice.h"

using namespace LKEngine::Vulkan;

VulkanQueue::VulkanQueue(VulkanDevice * device, uint32_t familyIndex, uint32_t queueIndex)
	:VulkanDeviceChild(device)
{
	vkGetDeviceQueue(device->GetRawDevice(), familyIndex, queueIndex, &vkQueue);
}
