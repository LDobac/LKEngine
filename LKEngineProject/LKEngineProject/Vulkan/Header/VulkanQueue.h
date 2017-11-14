#pragma once

#include <vulkan/vulkan.h>

#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanQueue
		: VulkanDeviceChild
	{
	private:
		VkQueue vkQueue;

		uint32_t familyIndex;
		uint32_t queueIndex;
	public:
		explicit VulkanQueue(VulkanDevice* device, uint32_t familyIndex, uint32_t queueIndex);

		VkQueue GetRawHandle() const;
	};
}