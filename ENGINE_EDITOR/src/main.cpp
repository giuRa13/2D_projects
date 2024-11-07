#define SDL_MAIN_HANDLED 1
//#define SOL_ALL_SAFETIES_ON 1
//#define NOMINMAX  //(only for Windows)
//set environment MALLOC_CHECK_ 2

#include "Application.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

int main(int argc, char* argv[])
{
#ifndef NDEBUG
#ifdef _WIN32
	ShowWindow( GetConsoleWindow(), SW_SHOW );
//#else
	// ADD Linux stuff
#endif
#else
#ifdef _WIN32
	ShowWindow( GetConsoleWindow(), SW_HIDE );
//#else
	// ADD Linux stuff
#endif
#endif

    auto& app = ENGINE_EDITOR::Application::GetInstance();
    app.Run();


    return 0;
}