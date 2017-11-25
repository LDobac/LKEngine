#include "../Header/Entity.h"

USING_LK_SPACE

Entity::Entity()
{ }

Entity::~Entity()
{ }

void Entity::Update()
{
}

void Entity::Render(const VkCommandBuffer& cmdBuffer)
{
}

std::string Entity::GetName() const
{
	return name;
}

glm::vec3 Entity::GetPosition() const
{
	return position;
}

glm::vec3 Entity::GetRotation() const
{
	return rotation;
}

glm::vec3 Entity::GetScale() const
{
	return scale;
}

void Entity::SetName(const std::string & name)
{
	this->name = name;
}

void Entity::SetPosition(const glm::vec3 & newPos)
{
	position = newPos;
}

void Entity::SetRotation(const glm::vec3 & newRot)
{
	rotation = newRot;
}

void Entity::SetScale(const glm::vec3 & newScale)
{
	scale = newScale;
}
