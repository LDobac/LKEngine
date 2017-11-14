#pragma once

namespace LKEngine::Vulkan
{
	class VulkanBaseInterface
	{
	public:
		VulkanBaseInterface() {}
		virtual ~VulkanBaseInterface() {}

		virtual void Init() {}
		virtual void Shutdown() {}
	};
}

