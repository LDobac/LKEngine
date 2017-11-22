#include "../Header/VulkanTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../Header/VulkanDevice.h"
#include "../Header/VulkanImage.h"
#include "../Header/VulkanBuffer.h"

USING_LK_VULKAN_SPACE

VulkanTexture::VulkanTexture(std::string path)
	:VulkanImage(VulkanDevice::GetInstance())
{
	CreateTexture(path, VulkanDevice::GetInstance()->GetSingleCommandPool());
	CreateSampler();
}

VulkanTexture::~VulkanTexture()
{
	vkDestroySampler(device->GetHandle(), sampler, nullptr);
	VulkanImage::Shutdown();
}

const VkSampler & VulkanTexture::GetSampler() const
{
	return sampler;
}

const VkImageView & VulkanTexture::GetImageView() const
{
	return imageView;
}

void VulkanTexture::CreateTexture(std::string path, VulkanSingleCommandPool * commandPool)
{
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	if (pixels == nullptr)
	{
		throw std::runtime_error("이미지 불러오기 실패");
	}

	VkDeviceSize imageSize = texWidth * texHeight * 4;
	VulkanBuffer* stagingBuffer = new VulkanBuffer(
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);
	stagingBuffer->Map(pixels);

	stbi_image_free(pixels);

	VulkanImage::Init(texWidth, texHeight,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		VK_IMAGE_ASPECT_COLOR_BIT);

	TransitionLayout(commandPool, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(stagingBuffer, this, commandPool);
	TransitionLayout(commandPool, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	SAFE_DELETE(stagingBuffer);
}

void VulkanTexture::CreateSampler()
{
	VkSamplerCreateInfo info = { };
	info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

	info.magFilter = VK_FILTER_LINEAR;
	info.minFilter = VK_FILTER_LINEAR;

	info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	info.anisotropyEnable = VK_TRUE;
	info.maxAnisotropy = 16;

	info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

	info.unnormalizedCoordinates = VK_FALSE;

	info.compareEnable = VK_FALSE;
	info.compareOp = VK_COMPARE_OP_ALWAYS;

	info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	info.mipLodBias = 0.0f;
	info.maxLod = 0.0f;
	info.minLod = 0.0f;

	Check_Throw(vkCreateSampler(device->GetHandle(), &info, nullptr, &sampler) != VK_SUCCESS, "텍스쳐 샘플러 생성 실패");
}
