#pragma once

#include <vulkan/vulkan.h>

namespace LKEngine::Vulkan
{
	class VulkanExtension;
	class VulkanDebug;

	class VulkanInstance
	{
	private:
		VkInstance vkInstance;

		VulkanDebug* debug;
	public :
		explicit VulkanInstance();
		~VulkanInstance();

		void Init(bool vaildationLayerOn);
		virtual void Shutdown();

		VkInstance GetHandle() const;
	};
}