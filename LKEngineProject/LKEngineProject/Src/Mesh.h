#pragma once

#include "Entity.h"

LK_SPACE_BEGIN

namespace Vulkan
{
	class VulkanBuffer;
	class VulkanMaterial;
	class VulkanDescriptorSet;
}

class Mesh
	: public Entity
{
private:
	Vulkan::VulkanBuffer* vertexBuffer;
	Vulkan::VulkanBuffer* indexBuffer;
	Vulkan::VulkanBuffer* uniformBuffer;
	Vulkan::VulkanDescriptorSet* descriptorSet;
	Vulkan::VulkanMaterial* material;
};

LK_SPACE_END