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
		VkDeviceMemory memory;
	public:
		explicit VulkanImage(VulkanDevice* device);
		explicit VulkanImage(VkImage& image, VulkanDevice* device);
		virtual ~VulkanImage();

		virtual void Init() override { }
		//Initalize only VkImageView
		void Init(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlag, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);
		void InitWithoutImage(VkFormat format, VkImageAspectFlags aspectFlags);

		virtual void Shutdown() override;
		//Delete only VkImageView
		void ShutdownWithoutImage();

		VkImage GetImage() const;
		VkImageView GetImageView() const;
	private:
		void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlag, VkMemoryPropertyFlags properties);
		void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);
		void CreateMemory(VkMemoryPropertyFlags properties);
	};
}