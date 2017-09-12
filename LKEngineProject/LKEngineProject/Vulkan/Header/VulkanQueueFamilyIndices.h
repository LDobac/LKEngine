#include <vector>
#include <vulkan/vulkan.hpp>

namespace LKEngine
{
	namespace Vulkan
	{
		class QueueFamilyIndices
		{
		private:
			int graphicsFamily;
			int presentFamily;

		public:
			QueueFamilyIndices() : graphicsFamily(-1), presentFamily(-1) { }

			inline bool FindQueueFamily(VkPhysicalDevice gpu,VkSurfaceKHR surface)
			{
				uint32_t queueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, nullptr);

				std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, queueFamilies.data());

				for (size_t i = 0; i < queueFamilies.size(); i++)
				{
					if ((queueFamilies[i].queueCount > 0) && (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) 
					{
						graphicsFamily = i;
					}

					VkBool32 presentSupport = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, surface, &presentSupport);

					if (queueFamilies[i].queueCount > 0 && presentSupport) 
					{
						presentFamily = i;
					}

					if (IsComplete()) return true;
				}

				return false;
			}

			inline bool IsComplete() 
			{
				return graphicsFamily >= 0 && presentFamily >= 0;
			}
		};
	}
}