#include "../Header/RendererComponent.h"

#include "../../Renderer/Header/VulkanDescriptorSet.h"
#include "../../Renderer/Header/VulkanBuffer.h"
#include "../../Renderer/Header/VulkanDevice.h"
#include "../Header/Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

USING_LK_SPACE

RendererComponent::RendererComponent(Entity* entity, MeshData& meshData)
	: Component(entity)
{
	CreateVertexBuffer(meshData.vertices);
	CreateIndexBuffer(meshData.indices);
	CreateUniformBuffer();
	CreateDescriptorSet();

	indicesCount = meshData.indices.size();

	//TODO : Materials
}

RendererComponent::~RendererComponent()
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

void RendererComponent::Begin()
{
	Vulkan::VulkanDevice::GetInstance()->RegisterRenderComponent(this);
}

void RendererComponent::Update()
{
	struct ModelUniformBuffer { glm::mat4 model; };
	ModelUniformBuffer model;

	model.model = glm::translate(model.model, GetEntity()->GetPosition());

	//TODO : 쿼터니온으로 변경
	model.model = glm::rotate(model.model, GetEntity()->GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
	model.model = glm::rotate(model.model, GetEntity()->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
	model.model = glm::rotate(model.model, GetEntity()->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));

	model.model = glm::scale(model.model, GetEntity()->GetScale());

	uniformBuffer->Map(&model);
}

void RendererComponent::End()
{
	Vulkan::VulkanDevice::GetInstance()->UnRegisterRenderComponent(this);
}

void RendererComponent::Render(const VkCommandBuffer & cmdBuffer)
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

		vkCmdDrawIndexed(cmdBuffer, static_cast<uint32_t>(indicesCount), 1, 0, 0, 0);
	}
}

void RendererComponent::AddMaterial(Material * material)
{
	materials.push_back(material);
}

void RendererComponent::CreateVertexBuffer(const std::vector<Vertex>& vertices)
{
	vertexBuffer = new Vulkan::VulkanBuffer(
		sizeof(Vertex) * vertices.size(),
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);
	vertexBuffer->CopyDataLocalMemory(vertices.data());
}

void RendererComponent::CreateIndexBuffer(const std::vector<uint32_t>& indices)
{
	indexBuffer = new Vulkan::VulkanBuffer(
		sizeof(indices[0])  * indices.size(),
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);
	indexBuffer->CopyDataLocalMemory(indices.data());
}

void RendererComponent::CreateUniformBuffer()
{
	uniformBuffer = new Vulkan::VulkanBuffer(
		sizeof(glm::mat4),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);
}

void RendererComponent::CreateDescriptorSet()
{
	descriptorSet = new Vulkan::VulkanDescriptorSet(
		EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultMesh"),
		EntityPool::GetInstance()->GetDescriptorPool());

	descriptorSet->AddBufferInfo(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uniformBuffer, 0, 0);

	descriptorSet->UpdateSets();
}