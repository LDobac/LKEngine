#pragma once

#include "VulkanBaseInterface.h"

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanDevice;

		class VulkanDeviceChild :
			public BaseInterface
		{
		private:
			VulkanDevice* device;
		public:
			explicit VulkanDeviceChild(VulkanDevice* device) :device(device) {}
			virtual ~VulkanDeviceChild() {}
		};
	}
}

