#define SDL_MAIN_HANDLED 1
//#define SOL_ALL_SAFETIES_ON 1
//#define NOMINMAX  //(only for Windows)

#include "Application.hpp"



int main(int argc, char* argv[])
{

    auto& app = ENGINE_EDITOR::Application::GetInstance();
    app.Run();


    return 0;
}