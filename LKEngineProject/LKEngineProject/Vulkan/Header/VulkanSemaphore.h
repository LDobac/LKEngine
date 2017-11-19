#pragma once

#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
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
}