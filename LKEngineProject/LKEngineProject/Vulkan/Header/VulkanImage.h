#pragma once

#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"
#include <vulkan/vulkan.hpp>

namespace LKEngine::Vulkan
{
	class VulkanImage
		: public IVulkanObject, VulkanDeviceChild
	{
	private:
		VkImage image;
		VkImageView imageView;
	public:
		explicit VulkanImage(VulkanDevice* device);
		explicit VulkanImage(VkImage& image, VulkanDevice* device);
		virtual ~VulkanImage();

		virtual void Init() override { }
		//Initalize only VkImageView
		void InitWithoutImage(VkFormat format, VkImageAspectFlags aspectFlags);

		virtual void Shutdown() override;
		//Delete only VkImageView
		void ShutdownWithoutImage();

	private:
		void CreateImage();
		void CreateImageView();
	};
}