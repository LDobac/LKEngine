#pragma once

#include "VulkanDeviceChild.h"
#include <vulkan/vulkan.hpp>

namespace LKEngine::Vulkan
{
	class VulkanImage
		: public VulkanDeviceChild
	{
	private:
		VkImage image;
		VkImageView imageView;
		VkDeviceMemory memory;
	public:
		explicit VulkanImage(VulkanDevice* device);
		explicit VulkanImage(VkImage& image, VulkanDevice* device);
		virtual ~VulkanImage();

		void Init(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlag, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);
		//Initalize only VkImageView
		void InitWithoutImage(VkFormat format, VkImageAspectFlags aspectFlags);

		virtual void Shutdown();
		//Delete only VkImageView
		void ShutdownWithoutImage();

		VkImage GetImage() const;
		const VkImageView& GetImageView() const;
	private:
		void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlag, VkMemoryPropertyFlags properties);
		void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);
		void CreateMemory(VkMemoryPropertyFlags properties);
	};
}