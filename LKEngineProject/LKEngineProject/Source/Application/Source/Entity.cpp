#include "../Header/Entity.h"

#include "../Header/Component.h"

USING_LK_SPACE

Entity::Entity()
	: position(0.0f, 0.0f, 0.0f),
	rotation(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f)
{
}

Entity::~Entity()
{ }

void Entity::Update()
{
	for (Component* component : components)
	{
		component->Update();
	}
}

void Entity::Render(const VkCommandBuffer& cmdBuffer) { }

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

void Entity::AddComponent(Component * newComponent)
{
	newComponent->Start();
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
