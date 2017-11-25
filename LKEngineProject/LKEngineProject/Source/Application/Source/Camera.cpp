#include "../Header/Camera.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "../../Renderer/Header/VulkanDevice.h"
#include "../../Renderer/Header/VulkanSwapchain.h"
#include "../../Renderer/Header/VulkanDescriptorSet.h"
#include "../../Renderer/Header/VulkanBuffer.h"

#include "../Header/EntityPool.h"

USING_LK_SPACE

Camera::Camera()
{
	look = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	uniformBuffer = new Vulkan::VulkanBuffer(
		sizeof(ViewProj),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE );

	CreateDescriptorSet();
}

Camera::~Camera()
{
	SAFE_DELETE(uniformBuffer);
	SAFE_DELETE(descriptorSet);
}

void Camera::Update()
{
	ViewProj viewproj = { };
	viewproj.view = GetViewMatrix();
	viewproj.proj = GetProjectionMatrix();
	viewproj.proj[1][1] *= -1;
	uniformBuffer->Map(&viewproj);
}

void Camera::CreateDescriptorSet()
{
	descriptorSet = new Vulkan::VulkanDescriptorSet(
		EntityPool::GetInstance()->GetDescriptorSetLayout("DefaultCamera"), 
		EntityPool::GetInstance()->GetDescriptorPool());
	descriptorSet->AddBufferInfo(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, uniformBuffer, 0, 0);
	descriptorSet->UpdateSets();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, look, up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	auto swapchain = Vulkan::VulkanDevice::GetInstance()->GetSwapchain();
	
	return glm::perspective(glm::radians(45.0f), swapchain->GetExtent().width / (float)swapchain->GetExtent().height, 0.1f, 10.0f) *= 1;
}

Vulkan::VulkanDescriptorSet * Camera::GetDescriptorSet()
{
	return descriptorSet;
}