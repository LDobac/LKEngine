#pragma once

#define LK_VERSION VK_MAKE_VERSION(1,0,0)
#define APPLICATION_VERSION VK_MAKE_VERSION(0,0,0)

#define ENGINE_NAME "LK Engine"

#define WINDOW_TITLE "Vulkan Engine"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

#define Console_Log(msg) do{ std::cout << msg << std::endl; } while(0);
#define Console_Log_If(expreesion, msg) do {if(expreesion){std::cout << msg << std::endl;}}while(0);

#include <iostream>

#define Check_Throw(expression,msg) if(expression) {throw std::runtime_error(msg);}
#define Check_Warning(expression,msg) if(expression) {std::cerr << msg << std::endl;} 

#define SAFE_DELETE(x) do{ delete x; x=nullptr; }while(0);
#define SAFE_ARR_DELETE(x) do { delete[] x; x=nullptr;}while(0);

#define CALLBACK __stdcall