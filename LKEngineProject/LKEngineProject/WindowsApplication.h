#pragma once

#include <GLFW/glfw3.h>
#include "VulkanDevice.h"

namespace LKEngine
{
	namespace Window
	{
		class WindowsApplication
		{
		private:
			GLFWwindow* window;

			Vulkan::VulkanDevice vkDevice;
		public:
			void Init();
			void CreateWindow(const int width,const int height);

			void MainLoop();

			void CleanUp();
		};
	}
}