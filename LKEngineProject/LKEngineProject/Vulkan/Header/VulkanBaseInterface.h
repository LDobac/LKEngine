#pragma once

namespace LKEngine::Vulkan
{
	class BaseInterface
	{
	public:
		BaseInterface() {}
		virtual ~BaseInterface() {}

		virtual void Init() {}
		virtual void Shutdown() {}
	};
}

