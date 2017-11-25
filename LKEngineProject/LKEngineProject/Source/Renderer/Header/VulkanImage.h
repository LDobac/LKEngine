#pragma once

#include "VulkanDeviceChild.h"
#include <vulkan/vulkan.hpp>

LK_VULKAN_SPACE_BEGIN

class VulkanBuffer;

class VulkanImage
	: public VulkanDeviceChild
{
protected:
	VkImage image;
	VkImageView imageView;
	VkDeviceMemory memory;

	size_t width;
	size_t height;

	VkFormat format;
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

	void TransitionLayout(VulkanSingleCommandPool* commandPool, VkImageLayout oldLayout, VkImageLayout newLayout);

	size_t GetWidth() const;
	size_t GetHeight() const;
	VkImage GetImage() const;
	VkFormat GetFormat() const;
	const VkImageView& GetImageView() const;
private:
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlag, VkMemoryPropertyFlags properties);
	void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);
	void CreateMemory(VkMemoryPropertyFlags properties);
protected:
	void CopyBufferToImage(VulkanBuffer* srcBuffer, VulkanImage* dstImagem, VulkanSingleCommandPool* commandPool);
};

LK_VULKAN_SPACE_END