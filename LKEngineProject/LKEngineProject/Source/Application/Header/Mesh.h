#pragma once

#include "Entity.h"
#include "VertexInformation.h"

LK_SPACE_BEGIN

namespace Vulkan
{
	class VulkanBuffer;
	class VulkanDescriptorSet;
}
class Material;

class Mesh
	: public Entity
{
private:
	size_t indicesCount;

	Vulkan::VulkanBuffer* vertexBuffer;
	Vulkan::VulkanBuffer* indexBuffer;

	Vulkan::VulkanBuffer* uniformBuffer;
	Vulkan::VulkanDescriptorSet* descriptorSet;

	std::vector<Material*> materials;
public:
	explicit Mesh(const MeshData& meshdata);
	virtual ~Mesh();

	virtual void Update() override;
	virtual void Render(const VkCommandBuffer& cmdBuffer) override;

	void AddMaterial(Material* material);
private:
	void CreateVertexBuffer(const std::vector<Vertex>& vertices);
	void CreateIndexBuffer(const std::vector<uint32_t>& indices);
	void CreateUniformBuffer();
	void CreateDescriptorSet();
};

LK_SPACE_END