#pragma once

namespace LKEngine::Vulkan
{
	class IVulkanObject
	{
	public:
		IVulkanObject() {}
		virtual ~IVulkanObject() {}

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
	};
}

