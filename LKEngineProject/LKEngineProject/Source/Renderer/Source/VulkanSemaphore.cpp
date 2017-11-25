#include "../Header/VulkanSemaphore.h"

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanSemaphore::VulkanSemaphore(VulkanDevice * device)
	:VulkanDeviceChild(device)
{
}

void VulkanSemaphore::Init()
{
	VkSemaphoreCreateInfo info = { };
	info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	Check_Throw(vkCreateSemaphore(device->GetHandle(), &info, nullptr, &semaphore) != VK_SUCCESS, "세마포어 생성 실패");
}

void VulkanSemaphore::Shutdown()
{
	vkDestroySemaphore(device->GetHandle(), semaphore, nullptr);
}

const VkSemaphore & VulkanSemaphore::GetHandle() const
{
	return semaphore;
}