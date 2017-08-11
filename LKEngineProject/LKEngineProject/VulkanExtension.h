#pragma once

#include <vector>

namespace LKEngine
{
	namespace Vulkan
	{
		const std::vector<const char*> vaildationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
		};

		class VulkanExtension
		{
		private:
			std::vector<const char*> vaildExtensions;

		public:
			void Init(bool isDebug);

			std::vector<const char*> GetExtensions() const;
		};
	}
}