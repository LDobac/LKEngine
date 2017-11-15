#pragma once

#include <vulkan/vulkan.h>

#include "IVulkanObject.h"

namespace LKEngine::Vulkan
{
	class VulkanExtension;
	class VulkanDebug;

	class VulkanInstance
		: public IVulkanObject
	{
	private:
		VkInstance vkInstance;

		VulkanDebug* debug;
	public :
		explicit VulkanInstance();
		~VulkanInstance();

		virtual void Init() override { }
		void Init(bool vaildationLayerOn);
		virtual void Shutdown() override;

		VkInstance operator*();
	};
}