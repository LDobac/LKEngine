#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Vulkan/Header/VulkanMaterial.h"
#include "../Vulkan/Header/VulkanPipeline.h"
#include "../Vulkan/Header/VulkanBuffer.h"
#include "../Vulkan/Header/VulkanDescriptorSet.h"
#include "../Vulkan/Header/VulkanDevice.h"

#include "Time.h"
#include "Camera.h"
#include "Scene.h"
#include "SceneManager.h"

USING_LK_SPACE

Mesh::Mesh(std::string meshpath, Vulkan::VulkanMaterial * material)
{
	this->material = material;
	LoadModel(meshpath);
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateUniformBuffer();
	CreateDescriptorSet();
}

Mesh::~Mesh()
{
	SAFE_DELETE(material);
	SAFE_DELETE(descriptorSet);
	SAFE_DELETE(vertexBuffer);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(uniformBuffer);
}

void Mesh::Update()
{
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), LKEngine::Time::GetTime() * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	uniformBuffer->Map(&model);
}

void Mesh::Render(const VkCommandBuffer& cmdBuffer)
{
	VkDeviceSize offsets = { 0 };
	vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vertexBuffer->GetBuffer(), &offsets);
	vkCmdBindIndexBuffer(cmdBuffer, indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
	
	Camera* camera = SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera();
	Vulkan::VulkanDescriptorSet* cameraDescriptorSet = camera->GetDescriptorSet();
	VkDescriptorSet descriptorSets[3] = {
		cameraDescriptorSet->GetHandle(),
		descriptorSet->GetHandle()
	};

	//TODO : 머터리얼 갯수만큼 루프
	descriptorSets[2] = material->GetDescriptorSet()->GetHandle();

	vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material->GetPipeline()->GetHandle());
	vkCmdBindDescriptorSets(
		cmdBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		material->GetPipeline()->GetLayout(),
		0,
		3,
		descriptorSets,
		0,
		nullptr
	);

	vkCmdDrawIndexed(cmdBuffer, indices.size(), 1, 0, 0, 0);
}

void Mesh::LoadModel(std::string meshpath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, meshpath.c_str())) {
		throw std::runtime_error(err);
	}

	std::unordered_map<Vulkan::Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vulkan::Vertex vertex = {};

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

void Mesh::CreateVertexBuffer()
{
	vertexBuffer = new Vulkan::VulkanBuffer(
		sizeof(Vulkan::Vertex) * vertices.size(),
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);
	vertexBuffer->CopyLocalMemory(vertices.data(), Vulkan::VulkanDevice::GetInstance()->GetSingleCommandPool());
}

void Mesh::CreateIndexBuffer()
{
	indexBuffer = new Vulkan::VulkanBuffer(
		sizeof(indices[0])  * indices.size(),
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);
	indexBuffer->CopyLocalMemory(indices.data(), Vulkan::VulkanDevice::GetInstance()->GetSingleCommandPool());
}

void Mesh::CreateUniformBuffer()
{
	uniformBuffer = new Vulkan::VulkanBuffer(
		sizeof(glm::mat4),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);
}

void Mesh::CreateDescriptorSet()
{
	descriptorSet = new Vulkan::VulkanDescriptorSet(
		EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultMesh"), 
		EntityPool::GetInstance()->GetDescriptorPool());
	descriptorSet->AddBufferInfo(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uniformBuffer, 0, 0);
	descriptorSet->UpdateSets();
}
