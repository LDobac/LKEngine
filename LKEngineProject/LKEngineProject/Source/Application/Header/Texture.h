#pragma once

#include "../../Renderer/Header/VulkanImage.h"

#include "Asset.h"

LK_SPACE_BEGIN

class Texture
	: public Asset, private Vulkan::VulkanImage
{
private:
	VkSampler sampler;
public:
	explicit Texture(const std::string& path);
	virtual ~Texture();

	const VkSampler& GetSampler() const;
	const VkImageView& GetImageView() const;
private:
	void CreateSampler();
};

LK_SPACE_END