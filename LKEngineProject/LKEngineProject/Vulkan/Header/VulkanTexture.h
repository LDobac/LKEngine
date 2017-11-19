#pragma once

#include "../Header/VulkanImage.h"
#include "../Header/VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanTexture
		: public VulkanImage
	{
	public:
		void Init(std::string path);
	};
}