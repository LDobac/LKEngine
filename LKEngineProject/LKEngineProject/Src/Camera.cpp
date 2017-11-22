#include "Camera.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "../Vulkan/Header/VulkanDevice.h"
#include "../Vulkan/Header/VulkanSwapchain.h"

#include "../Vulkan/Header/VulkanBuffer.h"

USING_LK_SPACE

Camera::Camera()
{
	position = glm::vec3(2.0f, 2.0f, 2.0f);
	look = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 0.0f, 1.0f); 
}

Camera::~Camera()
{ }

void Camera::Update()
{
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
