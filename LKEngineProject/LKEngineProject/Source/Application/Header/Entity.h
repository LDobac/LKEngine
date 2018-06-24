#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include "Object.h"

LK_SPACE_BEGIN

class Component;
class EntityPool;

class Entity : public Object
{
private:
	bool isRemoved;

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Entity* parent;

	std::vector<Component*> components;
	std::vector<Entity*> children;
public:
	explicit Entity();
	virtual ~Entity();

	virtual void Update();

#pragma region Properties

	virtual void SetPosition(const glm::vec3& newPos);
	virtual void SetRotation(const glm::vec3& newRot);
	virtual void SetScale(const glm::vec3& newScale);

	virtual glm::vec3 GetPosition() const;
	virtual glm::vec3 GetRotation() const;
	virtual glm::vec3 GetScale() const;

#pragma endregion

	void AddChild(Entity* newChild);

	void SetParent(Entity* newParent);

	Entity* GetParent() const;
	std::vector<Entity*> GetChildren() const;

	void AddComponent(Component* newComponent);

	template <typename C>
	inline C* GetComponent()
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			if (typeid(C) == typeid(components[i]))
			{
				return dynamic_cast<C>(components[i]);
			}
		}

		return nullptr;
	}
};

LK_SPACE_END;