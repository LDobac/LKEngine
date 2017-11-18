#pragma once

#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanSemaphore
		: public IVulkanObject,public VulkanDeviceChild
	{
	private:
		VkSemaphore semaphore;
	public:
		explicit VulkanSemaphore(VulkanDevice* device);

		virtual void Init() override;
		virtual void Shutdown() override;
	};
}