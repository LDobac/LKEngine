#pragma once

#include "../Header/VulkanImage.h"
#include "../Header/VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanTexture
		: public VulkanImage
	{
	private:
		VkSampler sampler;
	public:
		explicit VulkanTexture(VulkanDevice* device);

		void Init(std::string path,VulkanSingleCommandPool* commandPool);
		virtual void Shutdown() override;

		const VkSampler& GetSampler() const;
	private:
		void CreateTexture(std::string path, VulkanSingleCommandPool* commandPool);
		void CreateSampler();
	};
}