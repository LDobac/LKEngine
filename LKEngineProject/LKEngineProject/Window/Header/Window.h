#pragma once

#include <GLFW/glfw3.h>

namespace LKEngine::Window
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

		GLFWwindow* GetWindowHandle() const;
	};
}