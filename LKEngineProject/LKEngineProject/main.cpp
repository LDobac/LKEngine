#include "Application/Header/Application.h"

#include <cstdlib>
#include <crtdbg.h>
#include <map>

int main()
{
	using namespace LKEngine::Application;

	Application* app = new Application(800,600);

	app->Loop();

	delete app;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	return 0;
}