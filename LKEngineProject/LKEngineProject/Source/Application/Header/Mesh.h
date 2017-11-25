#pragma once

#include "Entity.h"

#include "../../Renderer/Header/VertexInformation.h"

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
	std::vector<Vulkan::Vertex> vertices;
	std::vector<uint32_t> indices;

	Vulkan::VulkanBuffer* vertexBuffer;
	Vulkan::VulkanBuffer* indexBuffer;

	Vulkan::VulkanBuffer* uniformBuffer;
	Vulkan::VulkanDescriptorSet* descriptorSet;

	//TODO : 머터리얼 여러개
	Material* material;
public:
	explicit Mesh(std::string meshpath, Material* material);
	virtual ~Mesh();

	virtual void Update() override;
	virtual void Render(const VkCommandBuffer& cmdBuffer) override;
private:
	void LoadModel(std::string meshpath);
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateUniformBuffer();
	void CreateDescriptorSet();
};

LK_SPACE_END