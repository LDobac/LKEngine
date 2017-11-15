#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "IWindow.h"
#include "WindowsWindow.h"
#include "../../Vulkan/Header/IVulkanObject.h"

namespace LKEngine::Window
{
	class WindowsWindow
		: IWindow, LKEngine::Vulkan::IVulkanObject
	{
	private:
		GLFWwindow* window;
	public:
		explicit WindowsWindow(const int width, const int height);
		virtual ~WindowsWindow();

		bool WindowShouldClose() override;

		void PollEvents() override;

		virtual void Init() override;
		virtual void Shutdown() override;

		GLFWwindow* GetWindowHandle() const;
	};
}