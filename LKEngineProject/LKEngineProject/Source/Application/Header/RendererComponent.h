#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>
#include "Component.h"

LK_SPACE_BEGIN

namespace Vulkan
{
	class VulkanBuffer;
	class VulkanDescriptorSet;
}
class Material;

class RenderComponent
	: public Component
{
protected:
	Vulkan::VulkanBuffer* vertexBuffer;
	Vulkan::VulkanBuffer* indexBuffer;

	Vulkan::VulkanBuffer* uniformBuffer;
	Vulkan::VulkanDescriptorSet* descriptorSet;

	std::vector<Material*> materials;
public:
	virtual ~RenderComponent();

	virtual void Update();
	virtual void Render(const VkCommandBuffer& cmdBuffer);

	virtual void AddMaterial(Material* material);
};

LK_SPACE_END