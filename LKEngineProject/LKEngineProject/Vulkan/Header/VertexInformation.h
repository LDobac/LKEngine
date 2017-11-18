#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

namespace LKEngine::Vulkan
{
	class Vertex
	{
	public:
		glm::vec3 pos;
		glm::vec3 color;
	};
	struct VertexInformation
	{
		static VkVertexInputBindingDescription BindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription = { };
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> AttributeDescription()
		{
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			return attributeDescriptions;
		}
	};
}