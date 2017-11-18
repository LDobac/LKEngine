#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "IWindow.h"
#include "WindowsWindow.h"

namespace LKEngine::Vulkan
{
	class VulkanDevice;
}

namespace LKEngine::Window
{
	class WindowsWindow
		: public IWindow
	{
	private:
		GLFWwindow* window;
	public:
		explicit WindowsWindow(const int width, const int height);
		virtual ~WindowsWindow();

		bool WindowShouldClose() override;

		void PollEvents() override;

		void Init(LKEngine::Vulkan::VulkanDevice* device);
		void Shutdown();

		GLFWwindow* GetWindowHandle() const;
	private:
		static void onWindowResized(GLFWwindow* window, int width, int height);
	};
}