#pragma once

namespace LKEngine
{
	namespace Vulkan
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

}

