#pragma once

#include "VulkanBaseInterface.h"
#include "VulkanDeviceChild.h"
#include <vulkan/vulkan.hpp>

namespace LKEngine::Vulkan
{
	class VulkanImage
		: public BaseInterface, VulkanDeviceChild
	{
	private:
		VkImage image;
		VkImageView imageView;
	public:
		explicit VulkanImage(VulkanDevice* device);
		explicit VulkanImage(VkImage& image, VulkanDevice* device);
		virtual ~VulkanImage();

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