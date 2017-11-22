#include "../Header/Application.h"

#include "../../Utility/Header/Macro.h"

#include "../../Window/Header/WindowsWindow.h"
#include "../../Vulkan/Header/VulkanDevice.h"

#include "../../Src/PipelineManager.h"
#include "../../Src/SceneManager.h"
#include "../../Src/EntityPool.h"
#include "../../Src/Time.h"

using namespace LKEngine::Application;

Application::Application(const int windowWidth, const int windowHeight)
{
	window = new Window::WindowsWindow(windowWidth,windowHeight);
	device = Vulkan::VulkanDevice::GetInstance();

	device->SetDebugMode(true);
	device->SetWindow(window);

	window->Init(device);
	device->Init();

	LKEngine::Time::Start();
}

Application::~Application()
{
	device->WaitIdle();

	PipelineManager::Release();
	SceneManager::Release();

	Vulkan::VulkanDevice::Release();
	window->Shutdown();

	SAFE_DELETE(window);
}

void LKEngine::Application::Application::Loop()
{
	while (!window->WindowShouldClose())
	{
		LKEngine::Time::Update();

		device->Update();

		//SceneManager::GetInstance()->Update();
		//EntityPool::GetInstance()->Update();

		device->Draw();

		window->PollEvents();
	}
}
