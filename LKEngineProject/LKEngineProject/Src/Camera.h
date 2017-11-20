#pragma once

#include "Entity.h"

LK_SPACE_BEGIN

class Camera
	: public Entity
{
private:
	glm::vec3 look;
	glm::vec3 up;
public:
	explicit Camera();
	virtual ~Camera();

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
};

LK_SPACE_END