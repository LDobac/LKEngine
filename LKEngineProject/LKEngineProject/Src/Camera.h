#pragma once

#include "Entity.h"

LK_SPACE_BEGIN

namespace Vulkan
{
	class VulkanBuffer;
}

class Camera
	: public Entity
{
private:
	glm::vec3 look;
	glm::vec3 up;

	Vulkan::VulkanBuffer* uniformBuffer;
public:
	explicit Camera();
	virtual ~Camera();

	virtual void Update() override;

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
};

LK_SPACE_END