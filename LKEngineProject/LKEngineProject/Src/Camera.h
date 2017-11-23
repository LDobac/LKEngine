#pragma once

#include <glm/glm.hpp>
#include "Entity.h"

LK_SPACE_BEGIN

namespace Vulkan
{
	class VulkanBuffer;
	class VulkanDescriptorSet;
}

class Camera
	: public Entity
{
	struct ViewProj
	{
		glm::mat4 view;
		glm::mat4 proj;
	};
private:
	glm::vec3 look;
	glm::vec3 up;

	Vulkan::VulkanBuffer* uniformBuffer;
	Vulkan::VulkanDescriptorSet* descriptorSet;
public:
	explicit Camera();
	virtual ~Camera();

	virtual void Update() override;

	Vulkan::VulkanDescriptorSet* GetDescriptorSet();

private:
	void CreateDescriptorSet();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
};

LK_SPACE_END