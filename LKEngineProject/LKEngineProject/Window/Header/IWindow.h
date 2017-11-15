#pragma once

#include <GLFW/glfw3.h>

namespace LKEngine::Window
{
	class IWindow
	{
	protected:
		int width;
		int height;
	public:
		explicit IWindow(const int width, const int height) : width(width), height(height) { }
		virtual ~IWindow() { }

		virtual bool WindowShouldClose() = 0;

		virtual void PollEvents() = 0;
	};
}