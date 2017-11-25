#include "../Header/Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Renderer/Header/VulkanPipeline.h"
#include "../../Renderer/Header/VulkanBuffer.h"
#include "../../Renderer/Header/VulkanDescriptorSet.h"
#include "../../Renderer/Header/VulkanDevice.h"

#include "../Header/Material.h"
#include "../Header/Time.h"
#include "../Header/Camera.h"
#include "../Header/Scene.h"
#include "../Header/SceneManager.h"

USING_LK_SPACE

Mesh::Mesh(const MeshData & meshdata)
{
	CreateVertexBuffer(meshdata.vertices);
	CreateIndexBuffer(meshdata.indices);
	indicesCount = meshdata.indices.size();
	CreateUniformBuffer();
	CreateDescriptorSet();
	//TODO : Material
}

Mesh::~Mesh()
{
	for (size_t i = 0; i < materials.size(); i++)
	{
		SAFE_DELETE(materials[i]);
	}
	SAFE_DELETE(descriptorSet);
	SAFE_DELETE(vertexBuffer);
	SAFE_DELETE(indexBuffer);
	SAFE_DELETE(uniformBuffer);
}

void Mesh::Update()
{
	struct ModelUniformBuffer { glm::mat4 model; };
	ModelUniformBuffer model;

	model.model = glm::translate(model.model, position);

	//TODO : 쿼터니온으로 변경
	model.model = glm::rotate(model.model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model.model = glm::rotate(model.model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model.model = glm::rotate(model.model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	model.model = glm::scale(model.model, scale);

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

	for (size_t i = 0; i < materials.size(); i++)
	{
		descriptorSets[2] = materials[i]->GetDescriptorSet()->GetHandle();

		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, materials[i]->GetPipeline()->GetHandle());
		vkCmdBindDescriptorSets(
			cmdBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			materials[i]->GetPipeline()->GetLayout(),
			0,
			3,
			descriptorSets,
			0,
			nullptr
		);

		vkCmdDrawIndexed(cmdBuffer, indicesCount, 1, 0, 0, 0);
	}
}

void Mesh::AddMaterial(Material * material)
{
	materials.push_back(material);
}

void Mesh::CreateVertexBuffer(const std::vector<Vertex>& vertices)
{
	vertexBuffer = new Vulkan::VulkanBuffer(
		sizeof(Vertex) * vertices.size(),
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);
	vertexBuffer->CopyDataLocalMemory(vertices.data());
}

void Mesh::CreateIndexBuffer(const std::vector<uint32_t>& indices)
{
	indexBuffer = new Vulkan::VulkanBuffer(
		sizeof(indices[0])  * indices.size(),
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);
	indexBuffer->CopyDataLocalMemory(indices.data());
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
