#pragma once

#include "VulkanBaseInterface.h"
#include "VulkanDeviceChild.h"
#include <vulkan/vulkan.hpp>

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanImage
			: public BaseInterface,VulkanDeviceChild
		{
		private:
			VkImage image;
			VkImageView imageView;
		public:
			explicit VulkanImage(VulkanDevice* device);
			explicit VulkanImage(VkImage& image, VulkanDevice* device);
			virtual ~VulkanImage();

			//VkImageView만 초기화(스왑 체인용)
			void InitWithoutImage(VkFormat format, VkImageAspectFlags aspectFlags);

			virtual void Shutdown() override;
			//VkImageView만 삭제(스왑 체인용)
			void ShutdownWithoutImage();

		private:
			void CreateImage();
			void CreateImageView();
		};
	}
}