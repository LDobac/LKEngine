#pragma once

#include <glm/glm.hpp>

#include "../Utility/Header/Macro.h"

LK_SPACE_BEGIN

class VulkanBuffer;

class Entity
{
protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
public:
	explicit Entity();
	virtual ~Entity();

	virtual void Update();

	virtual glm::vec3 GetPosition() const;
	virtual glm::vec3 GetRotation() const;
	virtual glm::vec3 GetScale() const;

	virtual void SetPosition(const glm::vec3& newPos);
	virtual void SetRotation(const glm::vec3& newRot);
	virtual void SetScale(const glm::vec3& newScale);
};

LK_SPACE_END;