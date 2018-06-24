#pragma once

#include <vector>

#include "../../Utility/Header/Macro.h"

LK_SPACE_BEGIN

namespace Vulkan
{
	class VulkanDescriptorSetLayout;
	class VulkanDescriptorSet;
	class VulkanPipeline;
}

class Texture;

class Material
{
private:
	std::vector<Texture*> textures;

	Vulkan::VulkanDescriptorSet* descriptorSet;
	Vulkan::VulkanPipeline* pipeline;
public:
	explicit Material(Vulkan::VulkanPipeline* pipeline);
	~Material();

	void AddTexture(Texture* texture);

	Vulkan::VulkanDescriptorSet* GetDescriptorSet() const;
	Vulkan::VulkanPipeline* GetPipeline() const;
};

LK_SPACE_END