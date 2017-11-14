#pragma once

#include "VulkanDevice.h"

namespace LKEngine::Vulkan
{
	class VulkanDeviceChild
	{
	protected:
		VulkanDevice* device;
	public:
		explicit VulkanDeviceChild(VulkanDevice* device) :device(device) {}
		virtual ~VulkanDeviceChild() {}
	};
}

