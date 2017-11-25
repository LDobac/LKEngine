#include "Source/Application/Header/Application.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "TestScene.h"

int main()
{
	LKEngine::Application::GetInstance()->Start(800, 600, new TestScene(), true);

	LKEngine::Application::GetInstance()->Loop();

	LKEngine::Application::Release();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	return 0;
}