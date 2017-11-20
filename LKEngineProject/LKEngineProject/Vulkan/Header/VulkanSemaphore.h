#pragma once

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanSemaphore
	: public VulkanDeviceChild
{
private:
	VkSemaphore semaphore;
public:
	explicit VulkanSemaphore(VulkanDevice* device);

	virtual void Init();
	virtual void Shutdown();

	const VkSemaphore& GetHandle() const;
};

LK_VULKAN_SPACE_END