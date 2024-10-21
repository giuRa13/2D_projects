#define SDL_MAIN_HANDLED 1
//#define NOMINMAX  //(only for Windows)

#include "Application.hpp"


/*void GetOpenGLVersionInfo()
{
    std::cout <<"\nVendor: " <<glGetString(GL_VENDOR) <<std::endl;
    std::cout <<"Renderer: " <<glGetString(GL_RENDERER) <<std::endl;
    std::cout <<"Version: " <<glGetString(GL_VERSION) <<std::endl;
    std::cout <<"Shadong Language: " <<glGetString(GL_SHADING_LANGUAGE_VERSION) <<std::endl;
    std::cout <<"\n";
}*/


int main()
{

    auto& app = ENGINE_EDITOR::Application::GetInstance();
    app.Run();


    return 0;
}