#pragma once

#include <GLFW/glfw3.h>
#include "VulkanDevice.h"

namespace LKEngine
{
	namespace Window
	{
		class Window
		{
		private:
			GLFWwindow* window;

			int width;
			int height;
		public:
			explicit Window(const int width, const int height);
			~Window();

			bool WindowShouldClose();

			void PollEvents();
		};
	}
}