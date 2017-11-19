#include "../Header/VulkanTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../Header/VulkanImage.h"
#include "../Header/VulkanBuffer.h"

using namespace LKEngine::Vulkan;

void VulkanTexture::Init(std::string path)
{
	int texWidth,texHeight,texChannels;
	stbi_uc* pixels = stbi_load("Textures/texture.jpg",&texWidth,&texHeight,&texChannels,STBI_rgb_alpha);
	if (pixels == nullptr)
	{
		throw std::runtime_error("이미지 불러오기 실패");
	}
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	VulkanBuffer* stagingBuffer = new VulkanBuffer(device);
	stagingBuffer->Init(
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);
	stagingBuffer->Map(pixels);

	VulkanImage::Init(texWidth, texHeight,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0);

}
