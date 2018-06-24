#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

#include "Component.h"
#include "VertexInformation.h"

LK_SPACE_BEGIN

namespace Vulkan
{
	class VulkanBuffer;
	class VulkanDescriptorSet;
}
class Material;

class RendererComponent : public Component
{
private:
	size_t indicesCount;

	Vulkan::VulkanBuffer* vertexBuffer;
	Vulkan::VulkanBuffer* indexBuffer;

	Vulkan::VulkanBuffer* uniformBuffer;
	Vulkan::VulkanDescriptorSet* descriptorSet;

	std::vector<Material*> materials;
public:
	explicit RendererComponent(Entity* entity, MeshData& meshData);
	virtual ~RendererComponent();

	virtual void Begin() override;
	virtual void Update() override;
	virtual void End() override;

	virtual void Render(const VkCommandBuffer& cmdBuffer);

	void AddMaterial(Material* material);

private:
	void CreateVertexBuffer(const std::vector<Vertex>& vertices);
	void CreateIndexBuffer(const std::vector<uint32_t>& indices);
	void CreateUniformBuffer();
	void CreateDescriptorSet();
};

LK_SPACE_END