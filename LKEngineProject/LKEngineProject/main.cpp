#include "WindowsApplication.h"

#include <stdlib.h>

int main()
{
	LKEngine::Window::WindowsApplication app;

	app.Init();
	app.CreateWindow(800,600);

	app.MainLoop();

	app.CleanUp();

	system("pause");

	return 0;
}