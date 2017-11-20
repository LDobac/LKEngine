#include "../Header/VulkanMesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanMesh::VulkanMesh()
{
	vertexBuffer = new VulkanBuffer(VulkanDevice::GetInstance());
	indexBuffer = new VulkanBuffer(VulkanDevice::GetInstance());
	uniformBuffer = new VulkanBuffer(VulkanDevice::GetInstance());

	texture = new VulkanTexture();
}

VulkanMesh::~VulkanMesh()
{
	SAFE_DELETE(vertexBuffer);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(uniformBuffer);
	SAFE_DELETE(texture);
}

void VulkanMesh::Init(std::string meshPath, std::string texPath)
{
	LoadModel(meshPath);
	LoadTexture(texPath);
	CreateVertexBuffer(VulkanDevice::GetInstance()->GetSingleCommandPool());
	CreateIndexBuffer(VulkanDevice::GetInstance()->GetSingleCommandPool());
	CreateUniformBuffer();
}

void VulkanMesh::Shutdown()
{
	vertexBuffer->Shutdown();
	indexBuffer->Shutdown();
	uniformBuffer->Shutdown();
	texture->Shutdown();
}

const std::vector<Vertex>& VulkanMesh::GetVertices() const
{
	return vertices;
}

const std::vector<uint32_t>& VulkanMesh::GetIndices() const
{
	return indices;
}

VulkanBuffer * VulkanMesh::GetVertexBuffer() const
{
	return vertexBuffer;
}

VulkanBuffer * VulkanMesh::GetIndexBuffer() const
{
	return indexBuffer;
}

VulkanBuffer * VulkanMesh::GetUniformBuffer() const
{
	return uniformBuffer;
}

VulkanTexture * VulkanMesh::GetTexture() const
{
	return texture;
}

void VulkanMesh::LoadModel(std::string meshPath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, meshPath.c_str())) {
		throw std::runtime_error(err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

void VulkanMesh::LoadTexture(std::string texPath)
{
	texture->Init(texPath);
}

void VulkanMesh::CreateVertexBuffer(VulkanSingleCommandPool* commandPool)
{
	vertexBuffer->Init(
		sizeof(Vertex) * vertices.size(),
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_SHARING_MODE_EXCLUSIVE);
	vertexBuffer->CopyLocalMemory(vertices.data(), commandPool);
}

void VulkanMesh::CreateIndexBuffer(VulkanSingleCommandPool* commandPool)
{
	indexBuffer->Init(
		sizeof(indices[0])  * indices.size(),
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_SHARING_MODE_EXCLUSIVE);
	indexBuffer->CopyLocalMemory(indices.data(), commandPool);
}

void VulkanMesh::CreateUniformBuffer()
{
	uniformBuffer->Init(
		sizeof(UniformBufferObject),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);
}
