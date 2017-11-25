#pragma once

#include "../../Utility/Header/TSingleton.h"

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanDevice;
	}

	namespace Window
	{
		class WindowsWindow;
	}

	class Scene;
}

LK_SPACE_BEGIN

class Application
	: public TSingleton<Application>
{
private:
	Window::WindowsWindow* window;
	Vulkan::VulkanDevice* device;

	bool isExit;
public:
	Application();
	~Application();
		
	void Start(int width, int height, Scene* startScene, bool debugMode = false);

	void Exit();

	void Loop();
};

LK_SPACE_END

