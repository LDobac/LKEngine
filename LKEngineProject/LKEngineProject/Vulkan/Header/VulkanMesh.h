#pragma once

#include <vector>
#include "VulkanBuffer.h"
#include "VertexInformation.h"
#include "VulkanTexture.h"
#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanMesh
{
private:
	VulkanBuffer* vertexBuffer;
	VulkanBuffer* indexBuffer;
	VulkanBuffer* uniformBuffer;

	VulkanTexture* texture;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
public:
	VulkanMesh(std::string meshPath, std::string texPath);
	~VulkanMesh();

	const std::vector<Vertex>& GetVertices() const;
	const std::vector<uint32_t>& GetIndices() const;
	VulkanBuffer* GetVertexBuffer() const;
	VulkanBuffer* GetIndexBuffer() const;
	VulkanBuffer* GetUniformBuffer() const;
	VulkanTexture* GetTexture() const;
private:
	void LoadModel(std::string meshPath);
	void LoadTexture(std::string texPath);
	void CreateVertexBuffer(VulkanSingleCommandPool* commandPool);
	void CreateIndexBuffer(VulkanSingleCommandPool* commandPool);
	void CreateUniformBuffer();
};

LK_VULKAN_SPACE_END